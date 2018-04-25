// ReCrafted (c) 2016-2018 Always Too Late

#include "../GFXL.hpp"
#include "../GFXLConfig.h"
#include "RHI/RHIBase.h"
#include "RHI/DirectX12/RHIDirectX12.h"
#include "RHI/DirectX11/RHIDirectX11.h"

#include <thread>

#if _WIN32
#include <Windows.h>
#endif

namespace GFXL
{
	namespace Internal 
	{
		void Log(const char* message)
		{
#if _WIN32
            OutputDebugStringA("GFXL Log: ");
            OutputDebugStringA(message);
            OutputDebugStringA("\n");
#else
            std::cout << "GFXL Log: " << message << std::endl;
#endif
		}

		void Fatal(const char* message)
		{
#if _WIN32
			OutputDebugStringA("GFXL Fatal: ");
			OutputDebugStringA(message);
			OutputDebugStringA("\n");
#else
			std::cout << "GFXL Fatal: " << message << std::endl;
#endif
			throw;
			// exit(-1);
		}
	}

	// ==== COMMANDS ====


	// ==== HANDLE DEFINITIONS ====
	GFXL_DEFINE_HANDLE_ALLOCATOR(WindowHandle, GFXL_MAX_WINDOWS);
	GFXL_DEFINE_HANDLE_ALLOCATOR(RenderBufferHandle, GFXL_MAX_RENDER_BUFFERS);
    GFXL_DEFINE_HANDLE_ALLOCATOR(Texture2DHandle, GFXL_MAX_TEXTURES2D);
    GFXL_DEFINE_HANDLE_ALLOCATOR(ShaderHandle, GFXL_MAX_SHADERS);

	const std::thread::id MAIN_THREAD_ID = std::this_thread::get_id();

	// TODO: Check if API calls are using main-thread

	static bool m_running = false;
	static RHI::RHIBase* m_renderer;

	static CommandList* g_commandList;

	void Initialize(RendererAPI::_enum api, ResetFlags::_enum flags, Settings::_enum settings)
	{
		switch (api)
		{
#if GFXL_RENDERER_D3D12
		case RendererAPI::DirectX12:
			m_renderer = new RHI::RHIDirectX12;
			m_renderer->Initialize(settings, flags);
			g_commandList = &m_renderer->commandList;
			break;
#endif
#if GFXL_RENDERER_D3D11
		case RendererAPI::DirectX11:
            m_renderer = new RHI::RHIDirectX11;
            m_renderer->Initialize(settings, flags);
            g_commandList = &m_renderer->commandList;
            break;
#endif
#if GFXL_RENDERER_VULKAN
		case RendererAPI::Vulkan:
            break;
#endif
#if GFXL_RENDERER_METAL
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

		if(GFXL_CHECK_HANDLE(handle))
			Internal::Fatal("Failed to allocate WindowHandle.");

        if (GFXL_CHECK_HANDLE(renderBuffer))
            Internal::Fatal("Failed to allocate RenderBuffer.");

		m_renderer->CreateWindowHandle(handle, renderBuffer, windowHandle);

        WindowHandle_table[handle.idx].renderBuffer = renderBuffer;

		return handle;
	}

	void ApplyWindow(WindowHandle handle)
	{
		GFXL_VALIDATE_HANDLE(handle);
		
		Command_ApplyWindow command;
		command.window = handle;
		g_commandList->WriteCommand(&command);
	}

	RenderBufferHandle GetWindowRenderBuffer(WindowHandle handle)
	{
        GFXL_VALIDATE_HANDLE(handle);

		return WindowHandle_table[handle.idx].renderBuffer;
	}

	void DestroyWindow(WindowHandle handle)
	{
		GFXL_VALIDATE_HANDLE(handle);

		Command_DestroyWindow command;
		command.window = handle;
		g_commandList->WriteCommand(&command);

		FreeWindowHandle(handle);
	}

	RenderBufferHandle CreateRenderBuffer()
	{
        auto handle = AllocRenderBufferHandle();

        if (GFXL_CHECK_HANDLE(handle))
            Internal::Fatal("Failed to allocate RenderBuffer.");

        // TODO: create render buffer impls

		return handle;
	}

	void ApplyRenderBuffer(RenderBufferHandle handle)
	{
        GFXL_VALIDATE_HANDLE(handle);

        Command_ApplyRenderBuffer command;
        command.renderBuffer = handle;

        g_commandList->WriteCommand(&command);
	}

	void ClearRenderBuffer(RenderBufferHandle handle, Color color)
	{
        GFXL_VALIDATE_HANDLE(handle);

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

        if (GFXL_CHECK_HANDLE(handle))
            Internal::Fatal("Failed to allocate Texture2D.");

        m_renderer->CreateTexture2D(handle, width, height, textureFormat);

        return handle;
    }

    ShaderHandle CreateShader(const char* fileName)
    {
        auto handle = AllocShaderHandle();

        if (GFXL_CHECK_HANDLE(handle))
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
