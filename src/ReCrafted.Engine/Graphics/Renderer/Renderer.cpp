// ReCrafted (c) 2016-2018 Always Too Late

#include "Renderer.hpp"
#include "RendererConfig.h"
#include "RHI/RHIBase.h"
#include "RHI/DirectX12/RHIDirectX12.h"
#include "RHI/DirectX11/RHIDirectX11.h"

#include <thread>

#if _WIN32
#include <Windows.h>
#endif

namespace Renderer
{
	namespace Internal 
	{
		void Log(const char* message)
		{
#if _WIN32
            OutputDebugStringA("Renderer Log: ");
            OutputDebugStringA(message);
            OutputDebugStringA("\n");
#else
            std::cout << "Renderer Log: " << message << std::endl;
#endif
		}

		void Fatal(const char* message)
		{
#if _WIN32
			OutputDebugStringA("Renderer Fatal: ");
			OutputDebugStringA(message);
			OutputDebugStringA("\n");
#else
			std::cout << "Renderer Fatal: " << message << std::endl;
#endif
			throw;
			// exit(-1);
		}
	}

	// ==== COMMANDS ====


	// ==== HANDLE DEFINITIONS ====
	RENDERER_DEFINE_HANDLE_ALLOCATOR(WindowHandle, RENDERER_MAX_WINDOWS);
	RENDERER_DEFINE_HANDLE_ALLOCATOR(RenderBufferHandle, RENDERER_MAX_RENDER_BUFFERS);
    RENDERER_DEFINE_HANDLE_ALLOCATOR(Texture2DHandle, RENDERER_MAX_TEXTURES2D);
    RENDERER_DEFINE_HANDLE_ALLOCATOR(ShaderHandle, RENDERER_MAX_SHADERS);

	const std::thread::id MAIN_THREAD_ID = std::this_thread::get_id();

	// TODO: Check if API calls are using main-thread

	static bool m_running = false;
	static RHI::RHIBase* m_renderer;

	static CommandList* g_commandList;

	void Initialize(RendererAPI::_enum api, ResetFlags::_enum flags, Settings::_enum settings)
	{
		switch (api)
		{
#if RENDERER_RENDERER_D3D12
		case RendererAPI::DirectX12:
			m_renderer = new RHI::RHIDirectX12;
			m_renderer->Initialize(settings, flags);
			g_commandList = &m_renderer->commandList;
			break;
#endif
#if RENDERER_RENDERER_D3D11
		case RendererAPI::DirectX11:
            m_renderer = new RHI::RHIDirectX11;
            m_renderer->Initialize(settings, flags);
            g_commandList = &m_renderer->commandList;
            break;
#endif
#if RENDERER_RENDERER_VULKAN
		case RendererAPI::Vulkan:
            break;
#endif
#if RENDERER_RENDERER_METAL
        case RendererAPI::Metal:
            break;
#endif
		default:
			Internal::Fatal("Selected RendererAPI is not currently supported, yet.");
			break;
		}
		m_running = true;
	}

	bool IsInitialized()
	{
		return m_running;
	}

	void Shutdown()
	{
		m_running = false;
		m_renderer->Shutdown();
	}

	void Fullscreen(bool useFullScreen)
	{

	}

	void Frame()
	{
		m_renderer->Frame();
	}

	WindowHandle CreateWindowHandle(void* windowHandle)
	{
		// Create output
		auto handle = AllocWindowHandle();
        auto renderBuffer = AllocRenderBufferHandle();

		if(RENDERER_CHECK_HANDLE(handle))
			Internal::Fatal("Failed to allocate WindowHandle.");

        if (RENDERER_CHECK_HANDLE(renderBuffer))
            Internal::Fatal("Failed to allocate RenderBuffer.");

		m_renderer->CreateWindowHandle(handle, renderBuffer, windowHandle);

        WindowHandle_table[handle.idx].renderBuffer = renderBuffer;

		return handle;
	}

	void ApplyWindow(WindowHandle handle)
	{
		RENDERER_VALIDATE_HANDLE(handle);
		
		Command_ApplyWindow command;
		command.window = handle;
		g_commandList->WriteCommand(&command);
	}

	RenderBufferHandle GetWindowRenderBuffer(WindowHandle handle)
	{
        RENDERER_VALIDATE_HANDLE(handle);

		return WindowHandle_table[handle.idx].renderBuffer;
	}

	void DestroyWindow(WindowHandle handle)
	{
		RENDERER_VALIDATE_HANDLE(handle);

		Command_DestroyWindow command;
		command.window = handle;
		g_commandList->WriteCommand(&command);

		FreeWindowHandle(handle);
	}

	RenderBufferHandle CreateRenderBuffer()
	{
        auto handle = AllocRenderBufferHandle();

        if (RENDERER_CHECK_HANDLE(handle))
            Internal::Fatal("Failed to allocate RenderBuffer.");

        // TODO: create render buffer impls

		return handle;
	}

	void ApplyRenderBuffer(RenderBufferHandle handle)
	{
        RENDERER_VALIDATE_HANDLE(handle);

        Command_ApplyRenderBuffer command;
        command.renderBuffer = handle;

        g_commandList->WriteCommand(&command);
	}

	void ClearRenderBuffer(RenderBufferHandle handle, Color color)
	{
        RENDERER_VALIDATE_HANDLE(handle);

		Command_ClearRenderBuffer command;
		command.renderBuffer = handle;
		command.color = color;

		g_commandList->WriteCommand(&command);
	}

	void DestroyRenderBuffer(RenderBufferHandle handle)
	{
	}

    Texture2DHandle CreateTexture2D(uint16_t width, uint16_t height, TextureFormat::_enum textureFormat)
    {
        auto handle = AllocTexture2DHandle();

        if (RENDERER_CHECK_HANDLE(handle))
            Internal::Fatal("Failed to allocate Texture2D.");

        m_renderer->CreateTexture2D(handle, width, height, textureFormat);

        return handle;
    }

    ShaderHandle CreateShader(const char* fileName)
    {
        auto handle = AllocShaderHandle();

        if (RENDERER_CHECK_HANDLE(handle))
            Internal::Fatal("Failed to allocate Shader.");

        Command_CreateShader command;
        command.shader = handle;
        strcpy_s(command.fileName, fileName);

        g_commandList->WriteCommand(&command);

        return handle;
    }

    void ApplyShader(ShaderHandle handle, int passId)
    {
        Command_ApplyShader command;
        command.shader = handle;
        command.passId = static_cast<uint16_t>(passId);
        g_commandList->WriteCommand(&command);
    }

    void DestroyShader(ShaderHandle handle)
    {
        Command_DestroyShader command;
        command.shader = handle;
        g_commandList->WriteCommand(&command);
    }

    void SetFlag(ResetFlags::_enum flag, bool value)
    {

    }

    bool GetFlag(ResetFlags::_enum flag)
    {
        return false;
    }
}
