// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef RENDERER_H
#define RENDERER_H

#include "RendererDefines.h"
#include "RendererConfig.h"
#include "ReCrafted.h"

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
            VSync              = 1 << 3,
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

	/**
	 * \brief Initializes Renderer
	 * \param api The renderer API to be used, it can be set only once.
	 * \param settings The Renderer settings.
	 */
	RENDERER_FUNCTION(void)                     Initialize(RendererAPI::_enum api, ResetFlags::_enum flags, Settings::_enum settings);

	/**
	 * \brief Checks if Renderer is initialized.
	 * \return Returns true when Renderer is initialized.
	 */
	RENDERER_FUNCTION(bool)                     IsInitialized();

	/**
	 * \brief Shutdowns Renderer.
	 */
	RENDERER_FUNCTION(void)                     Shutdown();



	// ======== RENDERING - BASIC ========

	/**
	* \brief Ends current frame.
	*/
	RENDERER_FUNCTION(void)                     Frame();

	/**
	* \brief Creates new window handle.
	* 
	* \note Currently only one window is supported.
	*/
	RENDERER_FUNCTION(WindowHandle)             CreateWindowHandle(void* windowHandle);

	/**
	* \brief Sets given window as current.
	*/
	RENDERER_FUNCTION(void)                     ApplyWindow(WindowHandle handle);

	/**
	* \brief Gets render buffer of given window.
	* \param handle The window which render buffer will be returned.
	* \return The render buffer, or invalid handle when something went wrong
	* 
	* \note The returned RenderBuffer will be deleted when the window is destroyed -
	*  you cannot pass the RBH to DestroyRenderBuffer, it will throw fatal error.
	* \note Window's RenderBuffer does not support depth stencil! To use it, you will need
	*  to create own RenderBuffer with depth flag, then blit to the target window's RB.
	*/
	RENDERER_FUNCTION(RenderBufferHandle)       GetWindowRenderBuffer(WindowHandle handle);

	/**
	* \brief Destroys given window.
	*/
	RENDERER_FUNCTION(void)                     DestroyWindow(WindowHandle handle);

	// NOT IMPLEMENTED!
	RENDERER_FUNCTION(RenderBufferHandle)       CreateRenderBuffer(/*TODO*/);

	// NOT IMPLEMENTED!
	RENDERER_FUNCTION(void)                     ResizeRenderBuffer(/*TODO*/);

    /**
	 * \brief Sets given render buffer as current render target.
	 * \param handle The render buffer to be set.
	 */
	RENDERER_FUNCTION(void)                     ApplyRenderBuffer(RenderBufferHandle handle);

	/**
	 * \brief Clears given render buffer.
	 * \param handle The render buffer handle.
	 * \param color The color which will be used to clear the render buffer.
	 */
	RENDERER_FUNCTION(void)                     ClearRenderBuffer(RenderBufferHandle handle, Color color);

	// NOT IMPLEMENTED!
	RENDERER_FUNCTION(void)                     DestroyRenderBuffer(RenderBufferHandle handle);

    // NOT IMPLEMENTED!
    RENDERER_FUNCTION(Texture2DHandle)          CreateTexture2D(uint16_t width, uint16_t height, TextureFormat::_enum textureFormat);

    /**
     * \brief Loads shader and creates it's shader handle.
     * \param fileName The compiled shader file.
     * \return The created shader handle.
     */
    RENDERER_FUNCTION(ShaderHandle)             CreateShader(const char* fileName);

    /**
    * \brief Sets given shader as current.
    * \param handle The shader handle to be set.
    */
    RENDERER_FUNCTION(void)                     ApplyShader(ShaderHandle handle, int passId);

    /**
    * \brief Destroys given shader.
    */
    RENDERER_FUNCTION(void)                     DestroyShader(ShaderHandle handle);

	// ======== RUNTIME ========

	// NOT IMPLEMENTED!
	RENDERER_FUNCTION(void)                     SetFlag(ResetFlags::_enum flag, bool value);

    // NOT IMPLEMENTED!
    RENDERER_FUNCTION(bool)                     GetFlag(ResetFlags::_enum flag);
}

#endif // RENDERER_H

