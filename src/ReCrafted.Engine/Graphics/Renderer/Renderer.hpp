// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include "ReCrafted.h"
#include "RendererDefines.h"
#include "RendererConfig.h"

#include <functional>

namespace Renderer
{
    typedef void* RendererMemory;

    struct Settings
    {
        enum _enum : char
        {
            None = 0,
            Debug = 1 << 0,
            SingleThreaded = 1 << 1,
            BGRAFrameBuffer = 1 << 2
        };
    };

    RENDERER_ENUM(Settings);

    struct RenderFlags
    {
        enum _enum : uint
        {
            None = 0,
            TripleBuffered = 1 << 0,
            MSAAx2 = 1 << 1,
            MSAAx4 = 1 << 2,
            VSync = 1 << 3,

            DrawLineLists = 1 << 4,

            DrawWireFrame = 1 << 5,

            DepthStencil = 1 << 6,
            DepthTest = 1 << 7,

            AnisotropicX2 = 1 << 8,
            AnisotropicX4 = 1 << 9,
            AnisotropicX8 = 1 << 10,
            AnisotropicX16 = 1 << 11,

            RenderOverlay = 1 << 12,

            Count
        };
        
        static const uint Default = DepthTest | DepthStencil | VSync;
    };

    RENDERER_ENUM(RenderFlags);

    struct RendererAPI
    {
        enum _enum : char
        {
            DirectX11,
            DirectX12,
            Vulkan,
            Metal,

            Count
        };
    };

    RENDERER_ENUM(RendererAPI);

    struct TextureFormat
    {
        enum _enum : char
        {
            Unknown,

            // Color formats
            R1,
            A8,

            R8,
            R8I,
            R8U,
            R8S,

            R16,
            R16I,
            R16U,
            R16F,
            R16S,

            R32I,
            R32U,
            R32F,

            RG8,
            RG8I,
            RG8U,
            RG8S,

            RG16,
            RG16I,
            RG16U,
            RG16F,
            RG16S,

            RG32I,
            RG32U,
            RG32F,

            RGB8,
            RGB8I,
            RGB8U,
            RGB8S,

            BGRA8,

            RGBA8,
            RGBA8I,
            RGBA8U,
            RGBA8S,

            RGBA16,
            RGBA16I,
            RGBA16U,
            RGBA16F,
            RGBA16S,

            RGBA32I,
            RGBA32U,
            RGBA32F,

            R5G6B5,
            RGBA4,
            RGB5A1,
            RGB10A2,
            R11G11B10F,

            // Depth formats
            D16,
            D16F,
            D24,
            D24F,
            D24S8,
            D32,
            D32F,

            Count,
        };
    };

    RENDERER_ENUM(TextureFormat);

    static const int TextureFormatInfo[][1] =
    {
        {0}, // Unknown

        {1}, // R1
        {8}, // A8

        {8}, // R8
        {8}, // R8I
        {8}, // R8U
        {8}, // R8S

        {16}, // R16
        {16}, // R16I
        {16}, // R16U
        {16}, // R16F
        {16}, // R16S

        {32}, // R32I
        {32}, // R32U
        {32}, // R32F

        {16}, // RG8
        {16}, // RG8I
        {16}, // RG8U
        {16}, // RG8S

        {32}, // RG16
        {32}, // RG16I
        {32}, // RG16U
        {32}, // RG16F
        {32}, // RG16S

        {64}, // RG32I
        {64}, // RG32U
        {64}, // RG32F

        {24}, // RGB8
        {24}, // RGB8I
        {24}, // RGB8U
        {24}, // RGB8S

        {32}, // BGRA8

        {32}, // RGBA8
        {32}, // RGBA8I
        {32}, // RGBA8U
        {32}, // RGBA8S

        {64}, // RGBA16
        {64}, // RGBA16I
        {64}, // RGBA16U
        {64}, // RGBA16F
        {64}, // RGBA16S

        {128}, // RGBA32I
        {128}, // RGBA32U
        {128}, // RGBA32F

        {16}, // R5G6B5
        {16}, // RGBA4
        {16}, // RGB5A1
        {32}, // RGB10A2
        {32}, // RG11B10F

        {16}, // D16
        {16}, // D16F
        {24}, // D24
        {24}, // D24F
        {32}, // D24S8
        {32}, // D32
        {32}, // D32F

    };

    struct Color
    {
    public:
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        float a = 0.0f;

    public:
        Color()
        {
        }

        Color(float r, float g, float b, float a)
        {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }
    };

    // ======== OBJECT HANDLE DEFINES ========
    RENDERER_DEFINE_HANDLE(VertexBuffer);

    RENDERER_DEFINE_HANDLE(IndexBuffer);

    RENDERER_DEFINE_HANDLE(Shader);

    RENDERER_DEFINE_HANDLE_WITH_DESCRIPTOR(Texture2D);
        TextureFormat::_enum textureFormat = TextureFormat::Unknown;
    RENDERER_DEFINE_HANDLE_WITH_DESCRIPTOR_END();

    RENDERER_DEFINE_HANDLE_WITH_DESCRIPTOR(RenderBuffer)
        std::vector<Texture2DHandle> renderTextures = {};
        Texture2DHandle depthBuffer = {};
    RENDERER_DEFINE_HANDLE_WITH_DESCRIPTOR_END();

    RENDERER_DEFINE_HANDLE_WITH_DESCRIPTOR(Window)
        RenderBufferHandle renderBuffer = {};
    RENDERER_DEFINE_HANDLE_WITH_DESCRIPTOR_END();

    // ======== COMMON ========

    /// <summary>
    /// Initializes Renderer
    /// </summary>
    /// <param name="api">The renderer API to be used, it can be set only once.</param>
    /// <param name="flags">The renderer flags.</param>
    /// <param name="settings">The renderer settings.</param>
    RENDERER_FUNCTION(void) Initialize(RendererAPI::_enum api, Settings::_enum settings,
                                       RenderFlags::_enum flags = RenderFlags::_enum(RenderFlags::Default));

    /// <summary>
    /// Checks if Renderer is initialized.
    /// </summary>
    /// <returns>True, when Renderer is initialized.</returns>
    RENDERER_FUNCTION(bool) IsInitialized();

    /// <summary>
    /// Shutdows the renderer.
    /// </summary>
    RENDERER_FUNCTION(void) Shutdown();

    /// <summary>
    /// Allocates given amount of memory.
    /// The allocated memory is available only for ONE frame by default or the specified amount of frames (see: lifeTime).
    /// </summary>
    /// <param name="size">Amount of memory to allocate.</param>
    /// <param name="lifeTime">
    /// The amount of frames that must pass to automatically free the memory.
    /// When 0, the memory life time will be unlimited and it will be required to 
    /// release it by using Renderer::Free(...) function.
    /// </param>
    /// <returns>The allocated memory pointer.</returns>
    RENDERER_FUNCTION(RendererMemory) Allocate(size_t size, uint lifeTime = 1);

    /// <summary>
    /// Allocates given amount of memory.
    /// The allocated memory is available only for ONE frame or the specified amount.
    /// </summary>
    /// <param name="data">The initial data, this data will be used interally.</param>
    /// <param name="releaseFunc">Release function pointer, that will be used to release given memory when its life time reaches 0.</param>
    /// <param name="lifeTime">
    /// The amount of frames that must pass to automatically free the memory.
    /// When 0, the memory life time will be unlimited and it will be required to 
    /// release it by using Renderer::Free(...) function.
    /// </param>
    /// <returns>The allocated memory pointer.</returns>
    RENDERER_FUNCTION(RendererMemory) Allocate(void* data, std::function<void(void*, void*)> releaseFunc, void* userData = nullptr, uint lifeTime = 2);

    /// <summary>
    /// Frees given memory pointer.
    /// Warning: Only memory allocated using 'Allocate(...)' can be releases using this function.
    /// </summary>
    /// <param name="memory">The memory pointer to be released.</param>
    RENDERER_FUNCTION(void) Free(RendererMemory memory);

    /// <summary>
    /// Frees given memory pointer on the RenderThread - after the last command (eg.: UpdateVertexBuffer).
    /// Warning: This can be called only from the MainThread!
    /// Warning: Only memory allocated using 'Allocate(...)' (with lifeTime set to 0!) can be releases using this function.
    /// </summary>
    /// <param name="memory">The memory pointer to be released.</param>
    RENDERER_FUNCTION(void) QueueFree(RendererMemory memory);

    RENDERER_FUNCTION(void) SetFlag(RenderFlags::_enum flag, bool value);

    RENDERER_FUNCTION(void) SetFlags(RenderFlags::_enum flag);

    RENDERER_FUNCTION(bool) GetFlag(RenderFlags::_enum flag);

    // ======== RENDERING - BASIC ========

    /// <summary>
    /// Finalizes current frame and executes all threads or commands when single threaded.
    /// </summary>
    RENDERER_FUNCTION(void) Frame();

    /// <summary>
    /// Draws given amount of vertices from the current vertex buffer.
    /// </summary>
    RENDERER_FUNCTION(void) Draw(uint vertexCount);

    /// <summary>
    /// Draws given amount of indices from the current index buffer.
    /// </summary>
    RENDERER_FUNCTION(void) DrawIndexed(uint indexCount);

    /// <summary>
    /// Creates new window handle.
    /// Note: Currently only one window is supported.
    /// </summary>
    /// <returns>The created window.</returns>
    RENDERER_FUNCTION(WindowHandle) CreateWindowHandle(void* windowHandle);

    /// <summary>
    /// Sets given window as current.
    /// </summary>
    /// <param name="handle">The window handle to be set as current.</param>
    RENDERER_FUNCTION(void) ApplyWindow(WindowHandle handle);

    /// <summary>
    /// Resizes given window frame buffer.
    /// </summary>
    /// <param name="width">The new width of the window frame buffer.</param>
    /// <param name="height">The new height of the window frame buffer.</param>
    RENDERER_FUNCTION(void) ResizeWindow(WindowHandle handle, uint16_t width, uint16_t height);

    /// <summary>
    /// Gets render buffer of given window. 
    /// The returned RenderBuffer will be deleted when the window is destroyed -
    /// you cannot pass the RBH to DestroyRenderBuffer, it will throw fatal error.
    /// Window's RenderBuffer does not support depth stencil! To use it, you will need
    /// to create own RenderBuffer with depth flag, then blit to the target window's RB.
    /// </summary>
    /// <param name="handle">The window which render buffer will be returned.</param>
    /// <returns>The render buffer, or invalid handle when something went wrong.</returns>
    RENDERER_FUNCTION(RenderBufferHandle) GetWindowRenderBuffer(WindowHandle handle);

    /// <summary>
    /// Destroys given window.
    /// </summary>
    /// <param name="handle">The window handle.</param>
    RENDERER_FUNCTION(void) DestroyWindow(WindowHandle handle);

    /// <summary>
    /// Creates new render buffer from using texture formats.
    /// </summary>
    /// <param name="width">The width of the render buffer.</param>
    /// <param name="height">The height of the render buffer.</param>
    /// <param name="textures">The texture formats.</param>
    /// <param name="texturesCount">The amount of given texture formats.</param>
    /// <param name="depthFormat">The depth format, leave Unknown to not create depth buffer.</param>
    /// <returns>The created render buffer.</returns>
    RENDERER_FUNCTION(RenderBufferHandle) CreateRenderBuffer(uint16_t width, uint16_t height,
                                                             TextureFormat::_enum* textures, uint8_t texturesCount,
                                                             TextureFormat::_enum depthFormat = TextureFormat::Unknown);

    /// <summary>
    /// Resizes given render uffer.
    /// </summary>
    /// <param name="width">The new width of the render buffer.</param>
    /// <param name="height">The new height of the render buffer.</param>
    RENDERER_FUNCTION(void) ResizeRenderBuffer(RenderBufferHandle handle, uint16_t width, uint16_t height);

    /// <summary>
    /// Sets given render buffer as current render target.
    /// </summary>
    /// <param name="handle">The render buffer to be set.</param>
    RENDERER_FUNCTION(void) ApplyRenderBuffer(RenderBufferHandle handle);

    /// <summary>
    /// Clears given render buffer.
    /// </summary>
    /// <param name="handle">The render buffer handle.</param>
    /// <param name="color">The color which will be used to clear the render buffer.</param>
    /// <param name="depth">The depth value which will be used to fill the depth buffer.</param>
    RENDERER_FUNCTION(void) ClearRenderBuffer(RenderBufferHandle handle, Color color, bool depth = 1.0f);

    /// <summary>
    /// Destroys given render buffer.
    /// </summary>
    /// <param name="handle">The render buffer handle.</param>
    RENDERER_FUNCTION(void) DestroyRenderBuffer(RenderBufferHandle handle);

    /// <summary>
    /// Creates new VertexBuffer without memory initialization.
    /// </summary>
    /// <param name="vertexCount">The count of vertices.</param>
    /// <param name="vertexSize">The size of a single vertex.</param>
    /// <param name="dynamic">When true, this buffer will be allowed to be updated through UpdateVertexBuffer.</param>
    /// <param name="buffer">When non-null there will be allocated buffer (and this param will be pointer to it), 
    /// and it will be released after the buffer is uploaded to the GPU. Also, buffer is marked as DYNAMIC and to upload the data, 
    /// you must update data by calling UpdateVertexBuffer(...).</param>
    RENDERER_FUNCTION(VertexBufferHandle) CreateVertexBuffer(uint count, uint vertexSize, bool dynamic = false, RendererMemory* buffer = nullptr);

    /// <summary>
    /// Creates new VertexBuffer.
    /// </summary>
    /// <param name="vertexCount">The count of vertices.</param>
    /// <param name="vertexSize">The size of a single vertex.</param>
    /// <param name="data">The memory pointer.</param>
    /// <param name="dynamic">When true, this buffer will be allowed to be updated through UpdateVertexBuffer.</param>
    RENDERER_FUNCTION(VertexBufferHandle) CreateVertexBuffer(uint vertexCount, uint vertexSize, RendererMemory data,
                                                             bool dynamic = false);

    /// <summary>
    /// Updates dynamic vertex buffer.
    /// </summary>
    /// <param name="handle">The vertex buffer handle.</param>
    /// <param name="data">The memory pointer.</param>
    /// <param name="size">The total memory size.</param>
    /// <param name="offset">The destination memory offset.</param>
    RENDERER_FUNCTION(void) UpdateVertexBuffer(VertexBufferHandle handle, RendererMemory data, uint size, uint offset);

    /// <summary>
    /// Sets given vertex buffer as current.
    /// </summary>
    /// <param name="handle">The vertex buffer handle.</param>
    RENDERER_FUNCTION(void) ApplyVertexBuffer(VertexBufferHandle handle);

    /// <summary>
    /// Destroys given vertex buffer.
    /// </summary>
    /// <param name="handle">The vertex buffer handle.</param>
    RENDERER_FUNCTION(void) DestroyVertexBuffer(VertexBufferHandle handle);

    /// <summary>
    /// Creates new IndexBuffer without memory initialization.
    /// </summary>
    /// <param name="count">The count of indices.</param>
    /// <param name="is32bit">Specifies the passed single index size. By default it is 32 bit. When false, it is 16.</param>
    /// <param name="dynamic">When true, this buffer will be allowed to be updated through UpdateIndexBuffer.</param>
    /// <param name="buffer">When non-null there will be allocated buffer (and this param will be pointer to it), 
    /// and it will be released after the buffer is uploaded to the GPU. Also, buffer is marked as DYNAMIC and to upload the data, 
    /// you must update data by calling UpdateIndexBuffer(...).</param>
    RENDERER_FUNCTION(IndexBufferHandle) CreateIndexBuffer(uint count, bool is32bit = true, bool dynamic = false, RendererMemory* buffer = nullptr);

    /// <summary>
    /// Creates new IndexBuffer.
    /// </summary>
    /// <param name="indexCount">The count of indices.</param>
    /// <param name="data">The memory pointer.</param>
    /// <param name="is32bit">Specifies the passed single index size. By default it is 32 bit. When false, it is 16.</param>
    /// <param name="dynamic">When true, this buffer will be allowed to be updated through UpdateIndexBuffer.</param>
    RENDERER_FUNCTION(IndexBufferHandle) CreateIndexBuffer(uint indexCount, RendererMemory data, bool is32bit = true,
                                                           bool dynamic = false);

    /// <summary>
    /// Updates dynamic index buffer.
    /// </summary>
    /// <param name="handle">The index buffer handle.</param>
    /// <param name="data">The memory pointer.</param>
    /// <param name="size">The total memory size.</param>
    /// <param name="offset">The destination memory offset.</param>
    RENDERER_FUNCTION(void) UpdateIndexBuffer(IndexBufferHandle handle, RendererMemory data, uint size, uint offset);

    /// <summary>
    /// Sets given index buffer as current.
    /// </summary>
    /// <param name="handle">The index buffer handle.</param>
    RENDERER_FUNCTION(void) ApplyIndexBuffer(IndexBufferHandle handle);

    /// <summary>
    /// Destroys given index buffer.
    /// </summary>
    /// <param name="handle">The index buffer handle.</param>
    RENDERER_FUNCTION(void) DestroyIndexBuffer(IndexBufferHandle handle);

    /// <summary>
    /// Creates new Texture2D.
    /// </summary>
    /// <param name="width">The Width of the new texture.</param>
    /// <param name="height">The Height of the new texture.</param>
    /// <param name="mipLevels">The amount of mip maps of the new texture.</param>
    /// <param name="textureFormat">The format of the new texture.</param>
    /// <param name="data">Texture data (can be null, then you can upload data by using UpdateTexture2D function)</param>
    /// <param name="dataSize">Data size.</param>
    RENDERER_FUNCTION(Texture2DHandle) CreateTexture2D(uint16_t width, uint16_t height, uint8_t mipLevels,
                                                       TextureFormat::_enum textureFormat, RendererMemory data,
                                                       size_t dataSize, bool renderTargetFlag = false);

    /// <summary>
    /// Creates new Texture2D.
    /// </summary>
    /// <param name="width">The Width of the new texture.</param>
    /// <param name="height">The Height of the new texture.</param>
    /// <param name="textureFormat">The format of the new texture.</param>
    /// <param name="data">Texture data (can be null, then you can upload data by using UpdateTexture2D function)</param>
    /// <param name="dataSize">Data size.</param>
    RENDERER_FUNCTION(Texture2DHandle) CreateTexture2D(uint16_t width, uint16_t height,
                                                       TextureFormat::_enum textureFormat, RendererMemory data,
                                                       size_t dataSize);

    /// <summary>
    /// Creates new Texture2D.
    /// </summary>
    /// <param name="width">The Width of the new texture.</param>
    /// <param name="height">The Height of the new texture.</param>
    /// <param name="mipLevels">The amount of mip maps of the new texture.</param>
    /// <param name="textureFormat">The format of the new texture.</param>
    RENDERER_FUNCTION(Texture2DHandle) CreateTexture2D(uint16_t width, uint16_t height, uint8_t mipLevels,
                                                       TextureFormat::_enum textureFormat);

    /// <summary>
    /// Creates new Texture2D.
    /// </summary>
    /// <param name="width">The Width of the new texture.</param>
    /// <param name="height">The Height of the new texture.</param>
    /// <param name="textureFormat">The format of the new texture.</param>
    RENDERER_FUNCTION(Texture2DHandle) CreateTexture2D(uint16_t width, uint16_t height,
                                                       TextureFormat::_enum textureFormat);

    /// <summary>
    /// Sets given texture as current at given slot.
    /// </summary>
    /// <param name="handle">The texture handle.</param>
    /// <param name="slot">The texture target slot.</param>
    RENDERER_FUNCTION(void) ApplyTexture2D(Texture2DHandle handle, uint8_t slot);

    /// <summary>
    /// Resizes given texture2d.
    /// </summary>
    /// <param name="width">The new width of the texture2d.</param>
    /// <param name="height">The new height of the texture2d.</param>
    RENDERER_FUNCTION(void) ResizeTexture2D(Texture2DHandle handle, uint16_t width, uint16_t height);

    /// <summary>
    /// Destroys given texture.
    /// </summary>
    /// <param name="handle">The texture handle.</param>
    RENDERER_FUNCTION(void) DestroyTexture2D(Texture2DHandle handle);

    /// <summary>
    /// Loads shader and creates it's shader handle.
    /// </summary>
    /// <param name="fileName">The compiled shader file.</param>
    /// <returns>The created shader handle.</returns>
    RENDERER_FUNCTION(ShaderHandle) CreateShader(const char* fileName);

    /// <summary>
    /// Sets shader value of given field in a specified buffer.
    /// Maximal data size is 64 bytes!
    /// </summary>
    /// <param name="handle">The shader which will get new value.</param>
    /// <param name="bufferId">The buffer id.</param>
    /// <param name="fieldId">The field id.</param>
    /// <param name="data">The data pointer.</param>
    /// <param name="dataSize">The data size.</param>
    RENDERER_FUNCTION(void) SetShaderValue(ShaderHandle handle, int bufferId, int fieldId, void* data, size_t dataSize);

    /// <summary>
    /// Sets given shader pass as current.
    /// Updates all uniforms in shader program. Please make sure that you SetShaderValue before calling this.
    /// </summary>
    /// <param name="handle">The shader handle.</param>
    /// <param name="passId">The shader pass index.</param>
    RENDERER_FUNCTION(void) ApplyShader(ShaderHandle handle, int passId);

    /// <summary>
    /// Destroys given shader.
    /// </summary>
    /// <param name="handle">The shader handle.</param>
    RENDERER_FUNCTION(void) DestroyShader(ShaderHandle handle);

    // ======== RENDERING ========

    /// <summary>
    /// Draws render texture to the first slot/render texture of render buffer.
    /// Note: This will change current render buffer binding etc.
    /// </summary>
    /// <param name="destination">The destination render buffer.</param>
    /// <param name="source">The source texture handle.</param>
    /// <param name="customShader">The custom shader.</param>
    RENDERER_FUNCTION(void) BlitTexture(RenderBufferHandle destination, Texture2DHandle source,
                                        ShaderHandle customShader = {});

    /// <summary>
    /// Draws render textures to the render buffer using custom shader.
    /// Note: This will change current render buffer binding etc.
    /// </summary>
    /// <param name="customShader">The custom shader.</param>
    /// <param name="destination">The destination render buffer.</param>
    /// <param name="sources">The source textures handles.</param>
    /// <param name="sourceCount">The amout of the source textures.</param>
    RENDERER_FUNCTION(void) BlitTextures(ShaderHandle customShader, RenderBufferHandle destination,
                                         Texture2DHandle* sources, uint8_t sourceCount, uint8_t baseSlot = 0u);
}

#endif // RENDERER_H
