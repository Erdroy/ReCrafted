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
    RENDERER_DEFINE_HANDLE_ALLOCATOR(ShaderHandle, RENDERER_MAX_SHADER_PROGRAMS);
    RENDERER_DEFINE_HANDLE_ALLOCATOR(VertexBufferHandle, RENDERER_MAX_VERTEX_BUFFERS);
    RENDERER_DEFINE_HANDLE_ALLOCATOR(IndexBufferHandle, RENDERER_MAX_INDEX_BUFFERS);

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

    RendererMemory Allocate(const size_t size)
    {
        return static_cast<RendererMemory>(new byte[size]);
    }

    void Free(RendererMemory memory)
    {
        delete[] static_cast<byte*>(memory);
    }

	void Frame()
	{
		m_renderer->Frame();
	}

    void Draw(uint vertexCount)
    {
        Command_Draw command;
        command.vertexCount = vertexCount;
        g_commandList->WriteCommand(&command);
    }

    void DrawIndexed(uint indexCount)
    {
        Command_DrawIndexed command;
        command.indexCount = indexCount;
        g_commandList->WriteCommand(&command);
    }

    WindowHandle CreateWindowHandle(void* windowHandle)
	{
		// Create output
        cvar handle = AllocWindowHandle();
        cvar renderBuffer = AllocRenderBufferHandle();

        RENDERER_VALIDATE_HANDLE(handle);
        RENDERER_VALIDATE_HANDLE(renderBuffer);

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
        cvar handle = AllocRenderBufferHandle();
        RENDERER_VALIDATE_HANDLE(handle);

        // TODO: create render buffer impl

		return handle;
	}

    VertexBufferHandle CreateVertexBuffer(uint count, uint vertexSize, bool dynamic)
    {
        return CreateVertexBuffer(count, vertexSize, nullptr, dynamic);
    }

    VertexBufferHandle CreateVertexBuffer(uint vertexCount, uint vertexSize, RendererMemory memory, bool dynamic)
    {
        cvar handle = AllocVertexBufferHandle();
        RENDERER_VALIDATE_HANDLE(handle);

        Command_CreateVertexBuffer command;
        command.handle = handle;
        command.vertexCount = vertexCount;
        command.vertexSize = vertexSize;
        command.memory = memory;
        command.dynamic = dynamic;

        g_commandList->WriteCommand(&command);

        return handle;
    }

    void UpdateVertexBuffer(VertexBufferHandle handle, uint count, uint offset, RendererMemory memory)
    {
        RENDERER_VALIDATE_HANDLE(handle);

    }

    void ApplyVertexBuffer(VertexBufferHandle handle)
    {
        RENDERER_VALIDATE_HANDLE(handle);

        Command_ApplyVertexBuffer command;
        command.handle = handle;
        g_commandList->WriteCommand(&command);
    }

    void DestroyVertexBuffer(VertexBufferHandle handle)
    {
        RENDERER_VALIDATE_HANDLE(handle);

        Command_DestroyVertexBuffer command;
        command.handle = handle;
        g_commandList->WriteCommand(&command);
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
        cvar handle = AllocTexture2DHandle();
        RENDERER_VALIDATE_HANDLE(handle);

        return handle;
    }

    ShaderHandle CreateShader(const char* fileName)
    {
        cvar handle = AllocShaderHandle();
        RENDERER_VALIDATE_HANDLE(handle);

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
