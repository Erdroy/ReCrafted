// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include "ReCrafted.h"
#include "RendererDefines.h"
#include "RendererConfig.h"

namespace Renderer
{
	typedef byte* RendererMemory;

	struct Settings
	{
		enum _enum : char
		{
			None                = 0,
			Debug               = 1 << 0,
			SingleThreaded      = 1 << 1,
			BGRAFrameBuffer     = 1 << 2
		};
	};
	RENDERER_ENUM(Settings);

    struct ResetFlags
    {
        enum _enum : char
        {
            None                = 0,
            TripleBuffered      = 1 << 0,
            MSAAx2              = 1 << 1,
            MSAAx4              = 1 << 2,
            VSync               = 1 << 3,

            DrawLineLists       = 1 << 4,
        };
    };
    RENDERER_ENUM(ResetFlags);

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

    struct ShaderType
    {
        enum _enum : char
        {
            PixelShader,
            VertexShader,
            ComputeShader,

            Count
        };
    };
    RENDERER_ENUM(ShaderType);

    struct VertexAttribute
    {
        enum _enum : char
        {
            Position,
            Normal,
            Tangent,
            BiTangent,
            Color0,
            Color1,
            Color2,
            Color3,
            TexCoord0,
            TexCoord1,
            TexCoord2,
            TexCoord3,
            TexCoord4,
            TexCoord5,
            TexCoord6,
            TexCoord7,

            Count
        };
    };
    RENDERER_ENUM(VertexAttribute);

    struct VertexAttributeType
    {
        enum _enum : char
        {
            UInt8,
            Int16,
            Half,
            Float,

            Count
        };
    };
    RENDERER_ENUM(VertexAttributeType);

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
            RGB9E5F,
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

	struct Color
	{
	public:
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 0.0f;

	public:
		Color() {}
		Color(float r, float g, float b, float a)
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}
	};

	namespace Internal
	{
		RENDERER_FUNCTION(void) Log(const char* message);
		RENDERER_FUNCTION(void) Fatal(const char* message);
	}


	// ======== OBJECT HANDLE DEFINES ========

	RENDERER_DEFINE_HANDLE(RenderBuffer);
    RENDERER_DEFINE_HANDLE(Texture2D);
    RENDERER_DEFINE_HANDLE(VertexBuffer);
    RENDERER_DEFINE_HANDLE(IndexBuffer);
    RENDERER_DEFINE_HANDLE(Shader);

    RENDERER_DEFINE_HANDLE_BEGIN(Window)
    RenderBufferHandle renderBuffer = {};
    RENDERER_DEFINE_HANDLE_END();


	// ======== COMMON ========

    /// <summary>
    /// Initializes Renderer
    /// </summary>
    /// <param name="api">The renderer API to be used, it can be set only once.</param>
    /// <param name="flags">The renderer flags.</param>
    /// <param name="settings">The renderer settings.</param>
	RENDERER_FUNCTION(void)                     Initialize(RendererAPI::_enum api, ResetFlags::_enum flags, Settings::_enum settings);

    /// <summary>
    /// Checks if Renderer is initialized.
    /// </summary>
    /// <returns>True, when Renderer is initialized.</returns>
	RENDERER_FUNCTION(bool)                     IsInitialized();

    /// <summary>
    /// Shutdows the renderer.
    /// </summary>
	RENDERER_FUNCTION(void)                     Shutdown();

    /// <summary>
    /// Allocates given amount of memory.
    /// The allocated memory is available only for ONE frame or the specified amount.
    /// </summary>
    /// <param name="size">Amount of memory to allocate.</param>
    /// <param name="lifeTime">
    /// The amount of frames that must pass to automatically free the memory.
    /// When 0, the memory life time will be unlimited and it will be required to 
    /// release it by using Renderer::Free(...) function.
    /// </param>
    /// <returns>The allocated memory pointer.</returns>
    RENDERER_FUNCTION(RendererMemory)           Allocate(size_t size, uint lifeTime = 1);

    /// <summary>
    /// Frees given memory pointer.
    /// 
    /// </summary>
    /// <param name="memory">The memory pointer to be released.</param>
    RENDERER_FUNCTION(void)                     Free(RendererMemory memory);

    // TODO: NOT IMPLEMENTED!
    RENDERER_FUNCTION(void)                     SetFlag(ResetFlags::_enum flag, bool value);

    // TODO: NOT IMPLEMENTED!
    RENDERER_FUNCTION(bool)                     GetFlag(ResetFlags::_enum flag);

	// ======== RENDERING - BASIC ========

    /// <summary>
    /// Finalizes current frame and executes all threads or commands when single threaded.
    /// </summary>
	RENDERER_FUNCTION(void)                     Frame();

    /// <summary>
    /// Draws given amount of vertices from the current vertex buffer.
    /// </summary>
    RENDERER_FUNCTION(void)                     Draw(uint vertexCount);

    /// <summary>
    /// Draws given amount of indices from the current index buffer.
    /// </summary>
    RENDERER_FUNCTION(void)                     DrawIndexed(uint indexCount);

    /// <summary>
    /// Creates new window handle.
    /// Note: Currently only one window is supported.
    /// </summary>
    /// <returns>The created window.</returns>
	RENDERER_FUNCTION(WindowHandle)             CreateWindowHandle(void* windowHandle);

    /// <summary>
    /// Sets given window as current.
    /// </summary>
    /// <param name="handle">The window handle to be set as current.</param>
	RENDERER_FUNCTION(void)                     ApplyWindow(WindowHandle handle);

    /// <summary>
    /// Gets render buffer of given window. 
    /// The returned RenderBuffer will be deleted when the window is destroyed -
    /// you cannot pass the RBH to DestroyRenderBuffer, it will throw fatal error.
    /// Window's RenderBuffer does not support depth stencil! To use it, you will need
    /// to create own RenderBuffer with depth flag, then blit to the target window's RB.
    /// </summary>
    /// <param name="handle">The window which render buffer will be returned.</param>
    /// <returns>The render buffer, or invalid handle when something went wrong.</returns>
	RENDERER_FUNCTION(RenderBufferHandle)       GetWindowRenderBuffer(WindowHandle handle);

    /// <summary>
    /// Destroys given window.
    /// </summary>
    /// <param name="handle">The window handle.</param>
	RENDERER_FUNCTION(void)                     DestroyWindow(WindowHandle handle);

	// TODO: NOT IMPLEMENTED!
	RENDERER_FUNCTION(RenderBufferHandle)       CreateRenderBuffer(/*TODO*/);
     
	// TODO: NOT IMPLEMENTED!
	RENDERER_FUNCTION(void)                     ResizeRenderBuffer(/*TODO*/);

    /// <summary>
    /// Sets given render buffer as current render target.
    /// </summary>
    /// <param name="handle">The render buffer to be set.</param>
    RENDERER_FUNCTION(void)                     ApplyRenderBuffer(RenderBufferHandle handle);

    /// <summary>
    /// Clears given render buffer.
    /// </summary>
    /// <param name="handle">The render buffer handle.</param>
    /// <param name="color">The color which will be used to clear the render buffer.</param>
    RENDERER_FUNCTION(void)                     ClearRenderBuffer(RenderBufferHandle handle, Color color);

    // NOT IMPLEMENTED!
    RENDERER_FUNCTION(void)                     DestroyRenderBuffer(RenderBufferHandle handle);

    /// <summary>
    /// Creates new VertexBuffer without memory initialization.
    /// </summary>
    /// <param name="vertexCount">The count of vertices.</param>
    /// <param name="vertexSize">The size of a single vertex.</param>
    /// <param name="dynamic">When true, this buffer will be allowed to be updated through UpdateVertexBuffer.</param>
    RENDERER_FUNCTION(VertexBufferHandle)       CreateVertexBuffer(uint count, uint vertexSize, bool dynamic = false);

    /// <summary>
    /// Creates new VertexBuffer.
    /// </summary>
    /// <param name="vertexCount">The count of vertices.</param>
    /// <param name="vertexSize">The size of a single vertex.</param>
    /// <param name="memory">The memory pointer. This memory will be freed after vertex buffer is created.</param>
    /// <param name="dynamic">When true, this buffer will be allowed to be updated through UpdateVertexBuffer.</param>
    RENDERER_FUNCTION(VertexBufferHandle)       CreateVertexBuffer(uint vertexCount, uint vertexSize, RendererMemory memory, bool dynamic = false);

    // TODO: NOT IMPLEMENTED!
    RENDERER_FUNCTION(void)                     UpdateVertexBuffer(VertexBufferHandle handle, uint count, uint offset, RendererMemory memory);

    /// <summary>
    /// Sets given vertex buffer as current.
    /// </summary>
    /// <param name="handle">The vertex buffer handle.</param>
    RENDERER_FUNCTION(void)                     ApplyVertexBuffer(VertexBufferHandle handle);

    /// <summary>
    /// Destroys given vertex buffer.
    /// </summary>
    /// <param name="handle">The vertex buffer handle.</param>
    RENDERER_FUNCTION(void)                     DestroyVertexBuffer(VertexBufferHandle handle);

    // TODO: NOT IMPLEMENTED!
    RENDERER_FUNCTION(Texture2DHandle)          CreateTexture2D(uint16_t width, uint16_t height, TextureFormat::_enum textureFormat);

    /// <summary>
    /// Loads shader and creates it's shader handle.
    /// </summary>
    /// <param name="fileName">The compiled shader file.</param>
    /// <returns>The created shader handle.</returns>
    RENDERER_FUNCTION(ShaderHandle)             CreateShader(const char* fileName);

    /// <summary>
    /// Sets given shader pass as current.
    /// </summary>
    /// <param name="handle">The shader handle.</param>
    /// <param name="passId">The shader pass index.</param>
    RENDERER_FUNCTION(void)                     ApplyShader(ShaderHandle handle, int passId);

    /// <summary>
    /// Destroys given shader.
    /// </summary>
    /// <param name="handle">The shader handle.</param>
    RENDERER_FUNCTION(void)                     DestroyShader(ShaderHandle handle);
}

#endif // RENDERER_H

