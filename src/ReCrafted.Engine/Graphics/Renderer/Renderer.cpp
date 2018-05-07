// ReCrafted (c) 2016-2018 Always Too Late

#include "Renderer.hpp"
#include "RendererConfig.h"
#include "RHI/RHIBase.h"
#include "RHI/DirectX12/RHIDirectX12.h"
#include "RHI/DirectX11/RHIDirectX11.h"

#include <thread>
#include <algorithm>

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

    class MemoryAllocation
	{
	public:
        uint ttl = 0u;
        RendererMemory memory = nullptr;
	};

	// ==== HANDLE DEFINITIONS ====
	RENDERER_DEFINE_HANDLE_ALLOCATOR(WindowHandle, RENDERER_MAX_WINDOWS);
	RENDERER_DEFINE_HANDLE_ALLOCATOR(RenderBufferHandle, RENDERER_MAX_RENDER_BUFFERS);
    RENDERER_DEFINE_HANDLE_ALLOCATOR(Texture2DHandle, RENDERER_MAX_TEXTURES2D);
    RENDERER_DEFINE_HANDLE_ALLOCATOR(ShaderHandle, RENDERER_MAX_SHADER_PROGRAMS);
    RENDERER_DEFINE_HANDLE_ALLOCATOR(VertexBufferHandle, RENDERER_MAX_VERTEX_BUFFERS);
    RENDERER_DEFINE_HANDLE_ALLOCATOR(IndexBufferHandle, RENDERER_MAX_INDEX_BUFFERS);

	static std::thread::id g_mainThreadId = {};
    static CommandList* g_commandList;

	static bool m_running = false;
	static RHI::RHIBase* m_renderer;

    RenderFlags::_enum m_renderFlags;

    ShaderHandle m_blitShader;
    VertexBufferHandle m_quadVB; 
    IndexBufferHandle m_quadIB;

#if RENDERER_MEMORY_AUTO_DEALLOC_ENABLE
    // we don't really want to use our Array<T> type here, 
    // vector will still use rpmalloc due to overloaded new/delete operators
    std::vector<MemoryAllocation> m_memoryAllocations = {};
#endif

    void UpdateMemory()
    {
#if RENDERER_MEMORY_AUTO_DEALLOC_ENABLE
        // we don't need any lock's here as everything happens on the main thread.

        // deallocate memory that is out of life time
        for (rvar memory : m_memoryAllocations)
        {
            if(memory.ttl == 0)
            {
                if(memory.memory)
                    Free(memory.memory);
                memory.memory = nullptr;
            }
        }

        // clean deallocated memory entries
        m_memoryAllocations.erase(std::remove_if(m_memoryAllocations.begin(), m_memoryAllocations.end(), [](MemoryAllocation& memory) 
        {
            return !memory.memory;
        }), m_memoryAllocations.end());

        // decrement lifetime of memory that is still alive
        for (rvar memory : m_memoryAllocations)
        {
            if (memory.ttl > 0) // sanity check
                memory.ttl--;
        }
#endif
    }

	void Initialize(RendererAPI::_enum api, RenderFlags::_enum flags, Settings::_enum settings)
	{
        // get main thread index
        g_mainThreadId = std::this_thread::get_id();

        m_renderFlags = flags;

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
            m_renderer = new RHI::RHIDirectX11();
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

        // load shaders
        m_blitShader = CreateShader("../assets/shaders/Blit.shader");

        // create quad mesh
        static float quadVertices[16] = {
        //   position     uv
            -1.0f,  1.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 1.0f, 0.0f,
             1.0f, -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 1.0f
        };

        static uint16_t quadIndices[6] = {
            0, 1, 2,
            3, 0, 2
        };

        m_quadVB = CreateVertexBuffer(4, sizeof(float) * 4, reinterpret_cast<RendererMemory>(&quadVertices), false);
        m_quadIB = CreateIndexBuffer(6, reinterpret_cast<RendererMemory>(&quadIndices), false);
	}

	bool IsInitialized()
	{
		return m_running;
	}

	void Shutdown()
	{
        CHECK_MAIN_THREAD();

        for (rvar memory : m_memoryAllocations)
        {
            if (memory.memory)
                Free(memory.memory);
            memory.memory = nullptr;
        }
        m_memoryAllocations.clear();

		m_running = false;
		m_renderer->Shutdown();
	}

    RendererMemory Allocate(const size_t size, uint lifeTime)
    {
        CHECK_MAIN_THREAD();

        cvar memory = static_cast<RendererMemory>(new byte[size]);
        
#if RENDERER_MEMORY_AUTO_DEALLOC_ENABLE
        if(lifeTime > 0)
        {
            // we don't need any lock's here as everything happens on the main thread.

            MemoryAllocation mem = {};
            mem.memory = memory;
            mem.ttl = lifeTime;
            m_memoryAllocations.emplace_back(mem);
        }
#endif

        return memory;
    }

    void Free(RendererMemory memory)
    {
        CHECK_MAIN_THREAD();

        // TODO: check if this memory exists in memAllocs

        delete[] static_cast<byte*>(memory);
    }

    void SetFlag(RenderFlags::_enum flag, bool value)
    {
        Command_SetFlag command;
        command.flag = flag;
        command.value = value;
        g_commandList->WriteCommand(&command);
    }

    bool GetFlag(RenderFlags::_enum flag)
    {
        return (m_renderFlags & flag) != 0;
    }

    void SetAnisotropicFiltering(AnisotropicFiltering::_enum filtering)
    {
    }

    void Frame()
	{
        CHECK_MAIN_THREAD();

		m_renderer->Frame();

        // update memory
        UpdateMemory();
	}

    void Draw(uint vertexCount)
    {
        CHECK_MAIN_THREAD();

        Command_Draw command;
        command.vertexCount = vertexCount;
        g_commandList->WriteCommand(&command);
    }

    void DrawIndexed(uint indexCount)
    {
        CHECK_MAIN_THREAD();

        Command_DrawIndexed command;
        command.indexCount = indexCount;
        g_commandList->WriteCommand(&command);
    }

    WindowHandle CreateWindowHandle(void* windowHandle)
	{
        CHECK_MAIN_THREAD();

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
        CHECK_MAIN_THREAD();
		RENDERER_VALIDATE_HANDLE(handle);
		
		Command_ApplyWindow command;
		command.window = handle;
		g_commandList->WriteCommand(&command);
	}

    void ResizeWindow(WindowHandle handle, uint16_t width, uint16_t height)
    {
        CHECK_MAIN_THREAD();
        RENDERER_VALIDATE_HANDLE(handle);

        // this is not as easy as the render textures,
        // we need to resize whole back-buffer, 
        // so swapchain and other things :sad:

        // write resize frame buffer command
        Command_ResizeFrameBuffer command;
        command.handle = GetWindowRenderBuffer(handle);
        command.windowHandle = handle;
        command.width = width;
        command.height = height;

        g_commandList->WriteCommand(&command);
    }

    RenderBufferHandle GetWindowRenderBuffer(WindowHandle handle)
	{
        CHECK_MAIN_THREAD();
        RENDERER_VALIDATE_HANDLE(handle);

		return WindowHandle_table[handle.idx].renderBuffer;
	}

	void DestroyWindow(WindowHandle handle)
	{
        CHECK_MAIN_THREAD();
		RENDERER_VALIDATE_HANDLE(handle);

		Command_DestroyWindow command;
		command.window = handle;
		g_commandList->WriteCommand(&command);

		FreeWindowHandle(handle);
	}

    RenderBufferHandle CreateRenderBuffer(uint16_t width, uint16_t height, TextureFormat::_enum* textures, uint8_t texturesCount, TextureFormat::_enum depthFormat)
    {
        CHECK_MAIN_THREAD();

        cvar handle = AllocRenderBufferHandle();
        RENDERER_VALIDATE_HANDLE(handle);

        Command_CreateRenderBuffer command;
        command.handle = handle;
        command.width = width;
        command.height = height;
        command.createDepthStencil = depthFormat != TextureFormat::Unknown;
        command.texturesCount = texturesCount;

        // copy texture formats
        for(var i = 0; i < texturesCount && i < RENDERER_MAX_RENDER_BUFFER_TARGETS; i ++)
        {
            cvar textureFormat = textures[i];
            cvar texture = CreateTexture2D(width, height, 1, textureFormat, nullptr, 0u, true);

            RENDERER_VALIDATE_HANDLE(texture);

            command.renderTargets[i] = texture;

            // set handle's render textures
            RenderBufferHandle_table[handle.idx].renderTextures.push_back(texture);
        }

        // create depth buffer
        if(depthFormat != TextureFormat::Unknown)
        {
            cvar texture = CreateTexture2D(width, height, 1, depthFormat, nullptr, 0u, true);

            RENDERER_VALIDATE_HANDLE(texture);

            command.depthTarget = texture;

            // set handle's depth buffer
            RenderBufferHandle_table[handle.idx].depthBuffer = texture;
        }

        g_commandList->WriteCommand(&command);

        // return fresh handle (as the current 'cvar handle' doesn't have render textures)
        return RenderBufferHandle_table[handle.idx];
    }

    void ResizeRenderBuffer(RenderBufferHandle handle, uint16_t width, uint16_t height)
    {
        // note: frame buffers do not have any render textures
        cvar isFrameBuffer = handle.renderTextures.empty();
        _ASSERT(isFrameBuffer == false); // Cannot resize Frame Buffer!

        // resize all textures
        for (rvar texture : handle.renderTextures)
            ResizeTexture2D(texture, width, height);

        if (RENDERER_CHECK_HANDLE(handle.depthBuffer))
            ResizeTexture2D(handle.depthBuffer, width, height);

        // write resize frame buffer command
        Command_ResizeRenderBuffer command;
        command.handle = handle;
        command.texturesCount = static_cast<uint8_t>(handle.renderTextures.size());
        command.depthTarget = handle.depthBuffer;
        command.width = width;
        command.height = height;

        for(var i = 0u; i < command.texturesCount; i ++)
            command.renderTargets[i] = handle.renderTextures[i];

        g_commandList->WriteCommand(&command);
    }

    void ApplyRenderBuffer(RenderBufferHandle handle)
    {
        CHECK_MAIN_THREAD();
        RENDERER_VALIDATE_HANDLE(handle);

        Command_ApplyRenderBuffer command;
        command.handle = handle;

        g_commandList->WriteCommand(&command);
    }

    void ClearRenderBuffer(RenderBufferHandle handle, Color color, bool depth)
    {
        CHECK_MAIN_THREAD();
        RENDERER_VALIDATE_HANDLE(handle);

        Command_ClearRenderBuffer command;
        command.handle = handle;
        command.color = color;
        command.depth = depth;

        g_commandList->WriteCommand(&command);
    }

    void DestroyRenderBuffer(RenderBufferHandle handle)
    {
        CHECK_MAIN_THREAD();
        RENDERER_VALIDATE_HANDLE(handle);

        // note: frame buffers do not have any render textures
        cvar isFrameBuffer = handle.renderTextures.empty();
        _ASSERT(isFrameBuffer == false); // Cannot destroy Frame Buffer!

        Command_DestroyRenderBuffer command;
        command.handle = handle;

        g_commandList->WriteCommand(&command);

        // destroy render textures
        std::for_each(handle.renderTextures.begin(), handle.renderTextures.end(), [] (Texture2DHandle handle){
            DestroyTexture2D(handle);
        });

        // destroy depth buffer if created
        if(handle.depthBuffer.idx > 0)
            DestroyTexture2D(handle.depthBuffer);

        // free render buffer handle
        FreeRenderBufferHandle(handle);
    }

    VertexBufferHandle CreateVertexBuffer(uint count, uint vertexSize, bool dynamic)
    {
        CHECK_MAIN_THREAD();

        return CreateVertexBuffer(count, vertexSize, nullptr, dynamic);
    }

    VertexBufferHandle CreateVertexBuffer(uint vertexCount, uint vertexSize, RendererMemory data, bool dynamic)
    {
        CHECK_MAIN_THREAD();

        cvar handle = AllocVertexBufferHandle();
        RENDERER_VALIDATE_HANDLE(handle);

        Command_CreateVertexBuffer command;
        command.handle = handle;
        command.vertexCount = vertexCount;
        command.vertexSize = vertexSize;
        command.memory = data;
        command.dynamic = dynamic;

        g_commandList->WriteCommand(&command);

        return handle;
    }

    void UpdateVertexBuffer(VertexBufferHandle handle, uint count, uint offset, RendererMemory data)
    {
        CHECK_MAIN_THREAD();
        RENDERER_VALIDATE_HANDLE(handle);

        // TODO: NOT IMPLEMENTED!
    }

    void ApplyVertexBuffer(VertexBufferHandle handle)
    {
        CHECK_MAIN_THREAD();
        RENDERER_VALIDATE_HANDLE(handle);

        Command_ApplyVertexBuffer command;
        command.handle = handle;
        g_commandList->WriteCommand(&command);
    }

    void DestroyVertexBuffer(VertexBufferHandle handle)
    {
        CHECK_MAIN_THREAD();
        RENDERER_VALIDATE_HANDLE(handle);

        Command_DestroyVertexBuffer command;
        command.handle = handle;
        g_commandList->WriteCommand(&command);

        FreeVertexBufferHandle(handle);
    }

    IndexBufferHandle CreateIndexBuffer(uint indexCount, RendererMemory data, bool is32bit, bool dynamic)
    {
        CHECK_MAIN_THREAD();

        cvar handle = AllocIndexBufferHandle();
        RENDERER_VALIDATE_HANDLE(handle);

        Command_CreateIndexBuffer command;
        command.handle = handle;
        command.indexCount = indexCount;
        command.indexSize = is32bit ? 32 : 16;
        command.memory = data;
        command.dynamic = dynamic;

        g_commandList->WriteCommand(&command);

        return handle;
    }

    void UpdateIndexBuffer(IndexBufferHandle handle, uint count, uint offset, RendererMemory data)
    {
        CHECK_MAIN_THREAD();
        RENDERER_VALIDATE_HANDLE(handle);

        // TODO: NOT IMPLEMENTED!
    }

    void ApplyIndexBuffer(IndexBufferHandle handle)
    {
        CHECK_MAIN_THREAD();
        RENDERER_VALIDATE_HANDLE(handle);

        Command_ApplyIndexBuffer command;
        command.handle = handle;
        g_commandList->WriteCommand(&command);
    }

    void DestroyIndexBuffer(IndexBufferHandle handle)
    {
        CHECK_MAIN_THREAD();
        RENDERER_VALIDATE_HANDLE(handle);

        Command_DestroyIndexBuffer command;
        command.handle = handle;
        g_commandList->WriteCommand(&command);

        FreeIndexBufferHandle(handle);
    }

    Texture2DHandle CreateTexture2D(uint16_t width, uint16_t height, uint8_t mipLevels, TextureFormat::_enum textureFormat, RendererMemory data, size_t dataSize, bool renderTargetFlag)
    {
        CHECK_MAIN_THREAD();

        cvar handle = AllocTexture2DHandle();
        RENDERER_VALIDATE_HANDLE(handle);

        // set texture format
        rvar texture = Texture2DHandle_table[handle.idx];
        texture.textureFormat = textureFormat;

        // create texture2d command
        Command_CreateTexture2D command;
        command.handle = handle;
        command.width = width;
        command.height = height;
        command.mipLevels = mipLevels;
        command.textureFormat = textureFormat;
        command.memory = data;
        command.dataSize = dataSize;
        command.renderTarget = renderTargetFlag;
        g_commandList->WriteCommand(&command);

        return Texture2DHandle_table[handle.idx];
    }

    Texture2DHandle CreateTexture2D(uint16_t width, uint16_t height, TextureFormat::_enum textureFormat, RendererMemory data, size_t dataSize)
    {
        return CreateTexture2D(width, height, 1u, textureFormat, data, dataSize);
    }

    Texture2DHandle CreateTexture2D(uint16_t width, uint16_t height, uint8_t mipLevels, TextureFormat::_enum textureFormat)
    {
        return CreateTexture2D(width, height, mipLevels, textureFormat, nullptr, 0u);
    }

    Texture2DHandle CreateTexture2D(uint16_t width, uint16_t height, TextureFormat::_enum textureFormat)
    {
        return CreateTexture2D(width, height, 1u, textureFormat, nullptr, 0u);
    }

    void ApplyTexture2D(Texture2DHandle handle, uint8_t slot)
    {
        CHECK_MAIN_THREAD();
        RENDERER_VALIDATE_HANDLE(handle);

        Command_ApplyTexture2D command;
        command.handle = handle;
        command.slot = slot;
        g_commandList->WriteCommand(&command);
    }

    void ResizeTexture2D(Texture2DHandle handle, uint16_t width, uint16_t height)
    {
        CHECK_MAIN_THREAD();
        RENDERER_VALIDATE_HANDLE(handle);

        Command_ResizeTexture2D command;
        command.handle = handle;
        command.width = width;
        command.height = height;
        g_commandList->WriteCommand(&command);
    }

    void DestroyTexture2D(Texture2DHandle handle)
    {
        CHECK_MAIN_THREAD();
        RENDERER_VALIDATE_HANDLE(handle);

        Command_DestroyTexture2D command;
        command.handle = handle;
        g_commandList->WriteCommand(&command);

        FreeTexture2DHandle(handle);
    }

    ShaderHandle CreateShader(const char* fileName)
    {
        CHECK_MAIN_THREAD();

        cvar handle = AllocShaderHandle();
        RENDERER_VALIDATE_HANDLE(handle);

        Command_CreateShader command;
        command.shader = handle;
        strcpy_s(command.fileName, fileName);

        g_commandList->WriteCommand(&command);

        return handle;
    }

    void SetShaderValue(ShaderHandle handle, int bufferId, int fieldId, void* data, size_t dataSize)
    {
        CHECK_MAIN_THREAD();
        RENDERER_VALIDATE_HANDLE(handle);

        _ASSERT(dataSize <= 64u); // float4x4 is the biggest type that we can pass

        Command_SetShaderValue command;
        command.shader = handle;
        command.bufferId = static_cast<uint8_t>(bufferId);
        command.fieldId = static_cast<uint8_t>(fieldId);
        command.dataSize = static_cast<uint8_t>(dataSize);
        g_commandList->WriteCommand(&command);
        g_commandList->Write(data, static_cast<uint32_t>(dataSize));
    }

    void ApplyShader(ShaderHandle handle, int passId)
    {
        CHECK_MAIN_THREAD();
        RENDERER_VALIDATE_HANDLE(handle);

        Command_ApplyShader command;
        command.shader = handle;
        command.passId = static_cast<uint16_t>(passId);
        g_commandList->WriteCommand(&command);
    }

    void DestroyShader(ShaderHandle handle)
    {
        CHECK_MAIN_THREAD();

        Command_DestroyShader command;
        command.shader = handle;
        g_commandList->WriteCommand(&command);

        FreeShaderHandle(handle);
    }

    void Blit(RenderBufferHandle destination, Texture2DHandle source, ShaderHandle customShader)
    {
        if(RENDERER_CHECK_HANDLE(customShader))
        {
            // bind custom shader
            ApplyShader(customShader, 0);
        }
        else
        {
            // bind shader
            ApplyShader(m_blitShader, 0);
        }

        // bind render target
        ApplyRenderBuffer(destination);

        // bind texture
        ApplyTexture2D(source, 0u);

        // apply vertex and index buffer
        ApplyVertexBuffer(m_quadVB);
        ApplyIndexBuffer(m_quadIB);

        // disable depth test
        cvar depthTest = GetFlag(RenderFlags::DepthTest);
        SetFlag(RenderFlags::DepthTest, false);

        // draw the quad
        DrawIndexed(6);

        // re-enable depth-test when needed
        SetFlag(RenderFlags::DepthTest, depthTest);

        // done.
    }
}
