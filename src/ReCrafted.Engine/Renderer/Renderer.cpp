// ReCrafted (c) 2016-2018 Always Too Late

#include "Renderer.hpp"
#include "RendererConfig.h"
#include "Core/Lock.h"
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
    class MemoryAllocation
    {
    public:
        uint ttl = 0u;
        void* userData = nullptr;
        void* memory = nullptr;
        std::function<void(void*, void*)> releaseFunc = {};
    };

    // ==== HANDLE DEFINITIONS ====
    RENDERER_DEFINE_HANDLE_ALLOCATOR(WindowHandle, RENDERER_MAX_WINDOWS);
    RENDERER_DEFINE_HANDLE_DESCRIPTOR_TABLE(Window, RENDERER_MAX_WINDOWS);

    RENDERER_DEFINE_HANDLE_ALLOCATOR(RenderBufferHandle, RENDERER_MAX_RENDER_BUFFERS);
    RENDERER_DEFINE_HANDLE_DESCRIPTOR_TABLE(RenderBuffer, RENDERER_MAX_RENDER_BUFFERS);

    RENDERER_DEFINE_HANDLE_ALLOCATOR(Texture2DHandle, RENDERER_MAX_TEXTURES2D);
    RENDERER_DEFINE_HANDLE_DESCRIPTOR_TABLE(Texture2D, RENDERER_MAX_TEXTURES2D);

    RENDERER_DEFINE_HANDLE_ALLOCATOR(ShaderHandle, RENDERER_MAX_SHADER_PROGRAMS);
    RENDERER_DEFINE_HANDLE_ALLOCATOR(VertexBufferHandle, RENDERER_MAX_VERTEX_BUFFERS);
    RENDERER_DEFINE_HANDLE_ALLOCATOR(IndexBufferHandle, RENDERER_MAX_INDEX_BUFFERS);

    static std::thread::id g_mainThreadId = {};
    static CommandList* g_commandList;

    static bool m_running = false;
    static RHI::RHIBase* m_renderer;

    const uint RenderFlags::Default;

    RenderFlags::_enum m_renderFlags;

    ShaderHandle m_blitShader;
    VertexBufferHandle m_quadVB;
    IndexBufferHandle m_quadIB;

#if RENDERER_MEMORY_AUTO_DEALLOC_ENABLE
    std::vector<MemoryAllocation> m_memoryAllocations = {};
    Lock m_memoryAllocationsLock = {};
#endif

    void FreeRendererMemory(void* ptr, void* userData)
    {
        Free(ptr);
    }

    void UpdateMemory()
    {
#if RENDERER_MEMORY_AUTO_DEALLOC_ENABLE
        // we don't need any lock's here as everything happens on the main thread.

        ScopeLock(m_memoryAllocationsLock);

        // deallocate memory that is out of life time
        for (rvar memory : m_memoryAllocations)
        {
            if (memory.ttl == 0)
            {
                if (memory.memory && memory.releaseFunc != nullptr)
                {
                    // release memory
                    memory.releaseFunc(static_cast<void*>(memory.memory), memory.userData);
                }

                memory.memory = nullptr;
            }
        }

        // clean deallocated memory entries
        m_memoryAllocations.erase(std::remove_if(m_memoryAllocations.begin(), m_memoryAllocations.end(),
                                                 [](MemoryAllocation& memory)
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

    void Initialize(RendererAPI::_enum api, Settings::_enum settings, RenderFlags::_enum flags)
    {
        // get main thread index
        g_mainThreadId = std::this_thread::get_id();

        m_renderFlags = flags;

        ASSERT(api == RendererAPI::DirectX11);

        m_renderer = new RHI::RHIDirectX11();
        m_renderer->Initialize(settings, flags);
        m_renderer->freeVertexBuffer = (RHI::RHIBase::vbhFreePtr)&FreeVertexBufferHandle;
        m_renderer->freeIndexBuffer = (RHI::RHIBase::ibhFreePtr)&FreeIndexBufferHandle;

        g_commandList = &m_renderer->commandList;
        m_running = true;

        // load shaders
        m_blitShader = CreateShader("../content/Shaders/Blit.rcasset");

        // create quad mesh
        static float quadVertices[16] = {
            //   position     uv
            -1.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 0.0f,
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
            ASSERT(memory.memory); // scream at Erdroy when he is way too dumb and frees the memory manually

            if (memory.memory && memory.releaseFunc != nullptr)
            {
                // release memory
                memory.releaseFunc(static_cast<void*>(memory.memory), memory.userData);
            }

            memory.memory = nullptr;
        }
        m_memoryAllocations.clear();

        m_running = false;
        m_renderer->Shutdown();
    }

    void GetContext(RHIContext* context)
    {
        m_renderer->GetContext(context);
    }

    TextureFormat::_enum DXGIFormatToTextureFormat(uint16_t dxgiFormat)
    {
        switch(dxgiFormat)
        {
        case 0:
            return TextureFormat::Unknown;

            // Color formats
        case 66: // DXGI_FORMAT_R1_UNORM
            return TextureFormat::R1;
        case 65: // DXGI_FORMAT_A8_UNORM
            return TextureFormat::A8;

        case 61: // DXGI_FORMAT_R8_UNORM
            return TextureFormat::R8;
        case 64: // DXGI_FORMAT_R8_SINT
            return TextureFormat::R8I;
        case 62: // DXGI_FORMAT_R8_UINT
            return TextureFormat::R8U;
        case 63: // DXGI_FORMAT_R8_SNORM
            return TextureFormat::R8S;

        case 56: // DXGI_FORMAT_R16_UNORM
            return TextureFormat::R16;
        case 59: // DXGI_FORMAT_R16_SINT
            return TextureFormat::R16I;
        case 57: // DXGI_FORMAT_R16_UINT
            return TextureFormat::R16U;
        case 54: // DXGI_FORMAT_R16_FLOAT
            return TextureFormat::R16F;
        case 58: // DXGI_FORMAT_R16_SNORM
            return TextureFormat::R16S;

        case 43: // DXGI_FORMAT_R32_SINT
            return TextureFormat::R32I;
        case 42: // DXGI_FORMAT_R32_UINT
            return TextureFormat::R32U;
        case 41: // DXGI_FORMAT_R32_FLOAT
            return TextureFormat::R32F;

        case 49: // DXGI_FORMAT_R8G8_UNORM
            return TextureFormat::RG8;
        case 52: // DXGI_FORMAT_R8G8_SINT
            return TextureFormat::RG8I;
        case 50: // DXGI_FORMAT_R8G8_UINT
            return TextureFormat::RG8U;
        case 51: // DXGI_FORMAT_R8G8_SNORM
            return TextureFormat::RG8S;

        case 35: // DXGI_FORMAT_R16G16_UNORM
            return TextureFormat::RG16;
        case 38: // DXGI_FORMAT_R16G16_SINT
            return TextureFormat::RG16I;
        case 36: // DXGI_FORMAT_R16G16_UINT
            return TextureFormat::RG16U;
        case 34: // DXGI_FORMAT_R16G16_FLOAT
            return TextureFormat::RG16F;
        case 37: // DXGI_FORMAT_R16G16_SNORM
            return TextureFormat::RG16S;

        case 18: // DXGI_FORMAT_R32G32_SINT
            return TextureFormat::RG32I;
        case 17: // DXGI_FORMAT_R32G32_UINT
            return TextureFormat::RG32U;
        case 16: // DXGI_FORMAT_R32G32_FLOAT
            return TextureFormat::RG32F;

        case 87: // DXGI_FORMAT_B8G8R8A8_UNORM
            return TextureFormat::BGRA8;
        case 91: // DXGI_FORMAT_B8G8R8A8_UNORM_SRGB
            return TextureFormat::BGRA8;

        case 28: // DXGI_FORMAT_R8G8B8A8_UNORM
            return TextureFormat::RGBA8;
        case 32: // DXGI_FORMAT_R8G8B8A8_SINT
            return TextureFormat::RGBA8I;
        case 30: // DXGI_FORMAT_R8G8B8A8_UINT
            return TextureFormat::RGBA8U;
        case 31: // DXGI_FORMAT_R8G8B8A8_SNORM
            return TextureFormat::RGBA8S;
        case 29: // DXGI_FORMAT_R8G8B8A8_UNORM_SRGB
            return TextureFormat::RGBA8sRGB;

        case 11: // DXGI_FORMAT_R16G16B16A16_UNORM
            return TextureFormat::RGBA16;
        case 14: // DXGI_FORMAT_R16G16B16A16_SINT
            return TextureFormat::RGBA16I;
        case 12: // DXGI_FORMAT_R16G16B16A16_UINT
            return TextureFormat::RGBA16U;
        case 10: // DXGI_FORMAT_R16G16B16A16_FLOAT
            return TextureFormat::RGBA16F;
        case 13: // DXGI_FORMAT_R16G16B16A16_SNORM
            return TextureFormat::RGBA16S;

        case 4: // DXGI_FORMAT_R32G32B32A32_SINT
            return TextureFormat::RGBA32I;
        case 3: // DXGI_FORMAT_R32G32B32A32_UINT
            return TextureFormat::RGBA32U;
        case 2: // DXGI_FORMAT_R32G32B32A32_FLOAT
            return TextureFormat::RGBA32F;

        case 85: // DXGI_FORMAT_B5G6R5_UNORM
            return TextureFormat::B5G6R5;
        case 115: // DXGI_FORMAT_B4G4R4A4_UNORM
            return TextureFormat::BGRA4;
        case 86: // DXGI_FORMAT_B5G5R5A1_UNORM
            return TextureFormat::BGR5A1;
        case 24: // DXGI_FORMAT_R10G10B10A2_UNORM
            return TextureFormat::RGB10A2;
        case 26: // DXGI_FORMAT_R11G11B10_FLOAT
            return TextureFormat::R11G11B10F;

            // Depth formats
        case 55: // DXGI_FORMAT_D16_UNORM
            return TextureFormat::D16;
        case 45: // DXGI_FORMAT_D24_UNORM_S8_UINT
            return TextureFormat::D24S8;
        case 40: // DXGI_FORMAT_D32_FLOAT
            return TextureFormat::D32F;

            // Compressed formats
        case 71: // DXGI_FORMAT_BC1_UNORM
            return TextureFormat::BC1U;
        case 74: // DXGI_FORMAT_BC2_UNORM
            return TextureFormat::BC2U;
        case 77: // DXGI_FORMAT_BC3_UNORM
            return TextureFormat::BC3U;
        case 80: // DXGI_FORMAT_BC4_UNORM
            return TextureFormat::BC4U;
        case 83: // DXGI_FORMAT_BC5_UNORM
            return TextureFormat::BC5U;
        case 95: // DXGI_FORMAT_BC6H_UF16
            return TextureFormat::BC6HUF16;
        case 96: // DXGI_FORMAT_BC6H_SF16
            return TextureFormat::BC6HSF16;
        case 98: // DXGI_FORMAT_BC7_UNORM
            return TextureFormat::BC7U;
        case 99: // DXGI_FORMAT_BC7_UNORM_SRGB
            return TextureFormat::BC7UsRGB;
        default:
            return TextureFormat::Unknown;
        }
    }

    void CalculateTextureInfo(const uint16_t width, const uint16_t height, const TextureFormat::_enum format, size_t* size, size_t* rowPitch, size_t* rowCount)
    {
        auto bcn = false;
        size_t bpe = 0;

        switch (format)
        {
        case TextureFormat::BC1U:
        case TextureFormat::BC4U:
            bcn = true;
            bpe = 8;
            break;

        case TextureFormat::BC2U:
        case TextureFormat::BC3U:
        case TextureFormat::BC5U:
        case TextureFormat::BC6HSF16:
        case TextureFormat::BC6HUF16:
        case TextureFormat::BC7U:
        case TextureFormat::BC7UsRGB:
            bcn = true;
            bpe = 16;
            break;

        default:
            break;
        }

        uint64_t numBytes;
        uint64_t rowBytes;
        uint64_t numRows;

        if (bcn)
        {
            uint64_t numBlocksWide = 0;
            if (width > 0)
            {
                numBlocksWide = std::max<uint64_t>(1u, (uint64_t(width) + 3u) / 4u);
            }
            uint64_t numBlocksHigh = 0;
            if (height > 0)
            {
                numBlocksHigh = std::max<uint64_t>(1u, (uint64_t(height) + 3u) / 4u);
            }
            rowBytes = numBlocksWide * bpe;
            numRows = numBlocksHigh;
            numBytes = rowBytes * numBlocksHigh;
        }
        else
        {
            const auto bpp = static_cast<size_t>(TextureFormatInfo[format][0]);
            ASSERT(bpp > 0);

            rowBytes = (uint64_t(width) * bpp + 7u) / 8u; // round up to nearest byte
            numRows = uint64_t(height);
            numBytes = rowBytes * height;
        }

        if (size)
            *size = numBytes;

        if (rowPitch)
            *rowPitch = rowBytes;

        if (rowCount)
            *rowCount = numRows;
    }

    size_t CalculateTextureRowPitch(const uint16_t width, const uint16_t height, const TextureFormat::_enum format)
    {
        size_t pitch;
        CalculateTextureInfo(width, height, format, nullptr, &pitch, nullptr);
        return pitch;
    }

    size_t CalculateTextureSize(const uint16_t width, const uint16_t height, const TextureFormat::_enum format)
    {
        size_t size;
        CalculateTextureInfo(width, height, format, &size, nullptr, nullptr);
        return size;
    }

    RendererMemory Allocate(const size_t size, uint lifeTime)
    {
        if (size == 0)
            return nullptr;

        cvar data = rc_malloc(size);
        return Allocate(data, std::function<void(void*, void*)>(&FreeRendererMemory), nullptr, lifeTime);
    }

    RendererMemory Allocate(void* data, std::function<void(void*, void*)> releaseFunc, void* userData, const uint lifeTime)
    {
        if (data == nullptr)
            return nullptr;

#if RENDERER_MEMORY_AUTO_DEALLOC_ENABLE
        if (lifeTime > 0)
        {
            ScopeLock(m_memoryAllocationsLock);

            MemoryAllocation mem = {};
            mem.memory = data;
            mem.ttl = lifeTime;
            mem.userData = userData;
            mem.releaseFunc = releaseFunc;
            m_memoryAllocations.emplace_back(mem);
        }
#endif

        return RendererMemory(data);
    }

    void Free(RendererMemory memory)
    {
        rc_free(static_cast<byte*>(memory));
    }

    void QueueFree(RendererMemory memory)
    {
        CHECK_MAIN_THREAD();

        Command_QueueFree command;
        command.memory = memory;
        g_commandList->WriteCommand(&command);
    }

    void SetFlag(RenderFlags::_enum flag, bool value)
    {
        m_renderFlags = RenderFlags::_enum(m_renderFlags & ~flag);

        Command_SetFlag command;
        command.flag = flag;
        command.value = value;
        g_commandList->WriteCommand(&command);
    }

    void SetFlags(RenderFlags::_enum flag)
    {
        Command_SetFlag command;
        command.flag = flag;
        g_commandList->WriteCommand(&command);
    }

    bool GetFlag(RenderFlags::_enum flag)
    {
        return (m_renderFlags & flag) != 0;
    }

    void GetRenderStatistics(RenderStatistics* stats)
    {
        m_renderer->GetRenderStatistics(stats);
    }

    void CaptureFrame(Texture2DHandle handle)
    {
        CHECK_MAIN_THREAD();

        Command_CaptureFrame command;
        command.targetTexture = handle;
        g_commandList->WriteCommand(&command);
    }

    void Frame()
    {
        CHECK_MAIN_THREAD();

        m_renderer->Frame();

        // Update memory
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

        rvar desc = GetWindowDescription(handle);
        desc.renderBuffer = renderBuffer;

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

        rvar desc = GetWindowDescription(handle);

        return desc.renderBuffer;
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

    void AddOnPresentCallback(const Action<void>& event)
    {
        m_renderer->callbacksBeforeRender.AddListener(event);
    }

    RenderBufferHandle CreateRenderBuffer(uint16_t width, uint16_t height, TextureFormat::_enum* textures,
                                          uint8_t texturesCount, TextureFormat::_enum depthFormat)
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

        rvar renderBufferDesc = GetRenderBufferDescription(handle);

        // copy texture formats
        for (var i = 0; i < texturesCount && i < RENDERER_MAX_RENDER_BUFFER_TARGETS; i ++)
        {
            cvar textureFormat = textures[i];
            cvar texture = CreateTexture2D(width, height, 0u, 1, textureFormat, nullptr, 0u, true);

            RENDERER_VALIDATE_HANDLE(texture);

            command.renderTargets[i] = texture;

            // set handle's render textures
            renderBufferDesc.renderTextures.push_back(texture);
        }

        // create depth buffer
        if (depthFormat != TextureFormat::Unknown)
        {
            cvar texture = CreateTexture2D(width, height, 0u, 1, depthFormat, nullptr, 0u, true);

            RENDERER_VALIDATE_HANDLE(texture);

            command.depthTarget = texture;

            // set handle's depth buffer
            renderBufferDesc.depthBuffer = texture;
        }

        g_commandList->WriteCommand(&command);

        // return fresh handle (as the current 'cvar handle' doesn't have render textures)
        return RenderBufferHandle_table[handle.idx];
    }

    void ResizeRenderBuffer(RenderBufferHandle handle, uint16_t width, uint16_t height)
    {
        rvar renderBufferDesc = GetRenderBufferDescription(handle);

        // note: frame buffers do not have any render textures
        cvar isFrameBuffer = renderBufferDesc.renderTextures.empty();
        ASSERT(isFrameBuffer == false); // Cannot resize Frame Buffer!

        // resize all textures
        for (rvar texture : renderBufferDesc.renderTextures)
            ResizeTexture2D(texture, width, height);

        if (RENDERER_CHECK_HANDLE(renderBufferDesc.depthBuffer))
            ResizeTexture2D(renderBufferDesc.depthBuffer, width, height);

        // write resize frame buffer command
        Command_ResizeRenderBuffer command;
        command.handle = handle;
        command.texturesCount = static_cast<uint8_t>(renderBufferDesc.renderTextures.size());
        command.depthTarget = renderBufferDesc.depthBuffer;
        command.width = width;
        command.height = height;

        for (var i = 0u; i < command.texturesCount; i ++)
            command.renderTargets[i] = renderBufferDesc.renderTextures[i];

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

        rvar renderBufferDesc = GetRenderBufferDescription(handle);

        // note: frame buffers do not have any render textures
        cvar isFrameBuffer = renderBufferDesc.renderTextures.empty();
        ASSERT(isFrameBuffer == false); // Cannot Destroy Frame Buffer!

        Command_DestroyRenderBuffer command;
        command.handle = handle;

        g_commandList->WriteCommand(&command);

        // Destroy render textures
        std::for_each(renderBufferDesc.renderTextures.begin(), renderBufferDesc.renderTextures.end(),
                      [](Texture2DHandle handle)
                      {
                          DestroyTexture2D(handle);
                      });

        // Destroy depth buffer if created
        if (renderBufferDesc.depthBuffer.idx > 0)
            DestroyTexture2D(renderBufferDesc.depthBuffer);

        // free render buffer handle
        FreeRenderBufferHandle(handle);
    }

    VertexBufferHandle CreateVertexBuffer(uint vertexCount, uint vertexSize, bool dynamic, RendererMemory* buffer)
    {
        CHECK_MAIN_THREAD();

        if(buffer != nullptr)
            *buffer = Allocate(vertexCount * vertexSize);

        return CreateVertexBuffer(vertexCount, vertexSize, buffer, buffer ? true : dynamic);
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

    VertexBufferHandle CreateVertexBufferSync(uint vertexCount, uint vertexSize, bool dynamic, RendererMemory buffer)
    {
        cvar handle = AllocVertexBufferHandle();
        RENDERER_VALIDATE_HANDLE(handle);

        m_renderer->CreateVertexBuffer(handle, vertexCount, vertexSize, dynamic, buffer);
        return handle;
    }

    void UpdateVertexBuffer(VertexBufferHandle handle, RendererMemory data, uint size, uint offset)
    {
        CHECK_MAIN_THREAD();
        RENDERER_VALIDATE_HANDLE(handle);

        Command_UpdateVertexBuffer command;
        command.handle = handle;
        command.memory = data;
        command.memorySize = size;
        command.memoryOffset = offset;

        g_commandList->WriteCommand(&command);
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

        //FreeVertexBufferHandle(handle); This is being called by RHI using 'freeVertexBuffer' function pointer
    }

    IndexBufferHandle CreateIndexBuffer(uint count, bool is32bit, bool dynamic, RendererMemory* buffer)
    {
        CHECK_MAIN_THREAD();

        if (buffer != nullptr)
            *buffer = Allocate(count * (is32bit ? 4u : 2u));

        return CreateIndexBuffer(count, nullptr, is32bit, buffer ? true : dynamic);
    }

    IndexBufferHandle CreateIndexBufferSync(uint count, bool is32bit, bool dynamic, RendererMemory buffer)
    {
        cvar handle = AllocIndexBufferHandle();
        RENDERER_VALIDATE_HANDLE(handle);
        m_renderer->CreateIndexBuffer(handle, count, is32bit, dynamic, buffer);
        return handle;
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

    void UpdateIndexBuffer(IndexBufferHandle handle, RendererMemory data, uint size, uint offset)
    {
        CHECK_MAIN_THREAD();
        RENDERER_VALIDATE_HANDLE(handle);

        Command_UpdateIndexBuffer command;
        command.handle = handle;
        command.memory = data;
        command.memorySize = size;
        command.memoryOffset = offset;

        g_commandList->WriteCommand(&command);
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

        //FreeIndexBufferHandle(handle); This is being called by RHI using 'freeIndexBuffer' function pointer
    }

    Texture2DHandle CreateTexture2D(uint16_t width, uint16_t height, uint16_t pitch, uint8_t mipLevels,
                                    TextureFormat::_enum textureFormat, RendererMemory data, size_t dataSize,
                                    bool renderTargetFlag, bool generateMips, TextureType::_enum textureType)
    {
        CHECK_MAIN_THREAD();

        cvar handle = AllocTexture2DHandle();
        RENDERER_VALIDATE_HANDLE(handle);

        // set texture format
        rvar renderBufferDesc = GetTexture2DDescription(handle);
        renderBufferDesc.textureFormat = textureFormat;

        // create texture2d command
        Command_CreateTexture2D command;
        command.handle = handle;
        command.width = width;
        command.height = height;
        command.pitch = pitch;
        command.mipLevels = mipLevels;
        command.textureFormat = textureFormat;
        command.textureType = textureType;
        command.memory = data;
        command.dataSize = dataSize;
        command.generateMips = generateMips;
        command.renderTarget = renderTargetFlag;
        g_commandList->WriteCommand(&command);

        return Texture2DHandle_table[handle.idx];
    }

    Texture2DHandle CreateTexture2D(uint16_t width, uint16_t height, TextureFormat::_enum textureFormat,
                                    RendererMemory data, size_t dataSize)
    {
        return CreateTexture2D(width, height, 0u, 1u, textureFormat, data, dataSize);
    }

    Texture2DHandle CreateTexture2D(uint16_t width, uint16_t height, uint8_t mipLevels,
                                    TextureFormat::_enum textureFormat, TextureType::_enum textureType)
    {
        return CreateTexture2D(width, height, 0u, mipLevels, textureFormat, nullptr, 0u, false, false, textureType);
    }

    Texture2DHandle CreateTexture2D(uint16_t width, uint16_t height, TextureFormat::_enum textureFormat, TextureType::_enum textureType)
    {
        return CreateTexture2D(width, height, 0u, 1u, textureFormat, nullptr, 0u, false, false, textureType);
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

    void ApplyTextureArray2D(Texture2DHandle* handles, uint8_t slot, uint8_t textureCount)
    {
        CHECK_MAIN_THREAD();
        ASSERT(textureCount > 0);
        ASSERT(textureCount <= 32);

        Command_ApplyTextureArray2D command;

        for(auto i = 0; i < textureCount; i ++)
        {
            const auto handle = handles[i];
            RENDERER_VALIDATE_HANDLE(handle);
            command.handles[i] = handle;
        }

        command.textureCount = textureCount;
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

    void UpdateTextureSubresource(Texture2DHandle handle, RendererMemory data, size_t dataSize, uint8_t subresourceId)
    {
        CHECK_MAIN_THREAD();
        RENDERER_VALIDATE_HANDLE(handle);

        Command_UpdateTexture2D command;
        command.handle = handle;
        command.data = data;
        command.dataSize = dataSize;
        command.subresourceId = subresourceId;
        g_commandList->WriteCommand(&command);
    }

    void UpdateTextureSubresourceSync(const Texture2DHandle handle, const RendererMemory data, const size_t dataSize, const uint8_t subresourceId)
    {
        CHECK_MAIN_THREAD();
        m_renderer->UpdateTextureSubresource(handle, data, dataSize, subresourceId);
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

    Texture2DHandle CreateRenderTexture(uint16_t width, uint16_t height, TextureFormat::_enum textureFormat, TextureType::_enum textureType)
    {
        return CreateTexture2D(width, height, 0u, 0u, textureFormat, nullptr, 0u, true, false, textureType);
    }

    void ApplyRenderTexture(Texture2DHandle handle, uint8_t slot)
    {
        CHECK_MAIN_THREAD();
        RENDERER_VALIDATE_HANDLE(handle);

        Command_ApplyRenderTexture2D command;
        command.handle = handle;
        command.slot = slot;
        g_commandList->WriteCommand(&command);
    }

    void ClearRenderTexture(Texture2DHandle handle, Color color)
    {
        CHECK_MAIN_THREAD();
        RENDERER_VALIDATE_HANDLE(handle);

        Command_ClearRenderTexture2D command;
        command.handle = handle;
        command.color = color;
        g_commandList->WriteCommand(&command);
    }

    void DestroyRenderTexture(Texture2DHandle handle)
    {
        DestroyTexture2D(handle);
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

        ASSERT(dataSize <= 64u); // float4x4 is the biggest type that we can pass

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

    void ExecuteTask(RenderTask* task)
    {
        CHECK_MAIN_THREAD();

        // Set rhi handle
        task->rhi = m_renderer;

        Command_ExecuteTask command;
        command.task = task;
        g_commandList->WriteCommand(&command);
    }

    void BlitTexture(RenderBufferHandle destination, Texture2DHandle source, ShaderHandle customShader)
    {
        CHECK_MAIN_THREAD();

        if (RENDERER_CHECK_HANDLE(customShader))
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
    
    void BlitTexture(Texture2DHandle destination, Texture2DHandle source, ShaderHandle customShader)
    {
        CHECK_MAIN_THREAD();

        if (RENDERER_CHECK_HANDLE(customShader))
        {
            // bind custom shader
            ApplyShader(customShader, 0);
        }
        else
        {
            // bind shader
            ApplyShader(m_blitShader, 0);
        }

        // bind render texture
        ApplyRenderTexture(destination, 0u);

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

    void BlitTextures(RenderBufferHandle destination, Texture2DHandle* sources,
                      uint8_t sourceCount, ShaderHandle customShader, uint8_t baseSlot)
    {
        CHECK_MAIN_THREAD();

        if (RENDERER_CHECK_HANDLE(customShader))
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

        // bind textures
        for (var i = 0u; i < sourceCount; i ++)
            ApplyTexture2D(sources[i], baseSlot + i);

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
    }

    void BlitTextures(Texture2DHandle destination, Texture2DHandle* sources,
        uint8_t sourceCount, ShaderHandle customShader, uint8_t baseSlot)
    {
        CHECK_MAIN_THREAD();

        if (RENDERER_CHECK_HANDLE(customShader))
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
        ApplyRenderTexture(destination, 0u);

        // bind textures
        for (var i = 0u; i < sourceCount; i++)
            ApplyTexture2D(sources[i], baseSlot + i);

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
    }
}
