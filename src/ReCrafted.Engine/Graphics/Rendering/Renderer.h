// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef RENDERER_H
#define RENDERER_H

// includes
#include "Core/Types.h"

// ReSharper disable IdentifierTypo
struct RendererType
{
	enum _enum
	{
		DirectX11,
		Vulkan
	};
};

struct ShaderType
{
	enum _enum
	{
		PixelShader,
		VertexShader,
		ComputeShader
	};
};

struct VertexAttrib
{
	enum _enum
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
struct VertexAttribType
{
	enum _enum
	{
		UInt8,
		Int16,
		Half,
		Float,

		Count
	};
};

/*struct VertexElem
{
public:
	VertexAttrib::_enum attrib = VertexAttrib::Position;
	int count = 0;
	VertexAttribType::_enum attribType = VertexAttribType::Float;

public:
	VertexElem() {}
	VertexElem(VertexAttrib::_enum attrib, int count, VertexAttribType::_enum attribType);
};

struct VertexDesc
{
	friend class D3D11Renderer;
	friend class VulkanRenderer;

private:
	VertexElem m_elemets[VertexAttrib::Count] = {};
	uint16_t m_count = 0u;

public:
	VertexDesc() {}

public:
	void add(VertexAttrib::_enum attrib, int count, VertexAttribType::_enum attribType)
	{
		_ASSERT(m_count < VertexAttrib::Count);

		m_elemets[m_count] = VertexElem(attrib, count, attribType);
		m_count++;
	}
};*/

struct Filtering
{
public:
	enum _enum
	{
		Point,
		Linear,
		Anisotropic
	};
};

struct Format
{
public:
	enum _enum
	{
		Unknown = 0,
		
		// R32 G32 B32 A32
		RGBA32_Float,
		RGBA32_UInt,
		RGBA32_SInt,

		// R32 G32 B32
		RGB32_Float,
		RGB32_UInt,
		RGB32_SInt,

		// R32 G32
		RG32_Float,
		RG32_UInt,
		RG32_SInt,

		// R32
		R32_Float,
		R32_UInt,
		R32_SInt,

		// B8 G8 R8 A8
		BGRA8_UNorm,
		BGRA8_UNormSRGB,

		// R8 G8 B8 A8
		RGBA8_UNorm,
		RGBA8_UNormSRGB,
		RGBA8_SNorm,
		RGBA8_UInt,
		RGBA8_SInt,

		// R8 G8
		RG8_UNorm,
		RG8_SNorm,
		RG8_UInt,
		RG8_SInt,

		// R8
		R8_UNorm,
		R8_SNorm,
		R8_UInt,
		R8_SInt,

		// Depth Formats
		D24_S8_UInt
	};
};
// ReSharper restore IdentifierTypo

struct handleBase
{
public:
	uint m_idx = 0;

public:
	handleBase() {}
	explicit handleBase(uint idx) : m_idx(idx) {}
};

#define INVALID_HANDLE handleBase(0u)

#define CREATE_HANDLE(name) \
	struct name##Handle : public handleBase\
	{\
	public:\
		name##Handle () {} \
		name##Handle (uint idx) { m_idx = idx; } \
	};\

CREATE_HANDLE(renderBuffer)
CREATE_HANDLE(texture2D)
CREATE_HANDLE(shader)
CREATE_HANDLE(vertexBuffer)
CREATE_HANDLE(indexBuffer)

class Renderer
{
private:

protected:
	Renderer() = default;
	virtual ~Renderer() = default;

protected:
	virtual void initialize(void* windowHandle, bool multithreaded) = 0;

public:
	/**
	* \brief Creates new vertex buffer.
	* \param vertexCount The vertex count.
	* \param vertexSize The vertex size (eg.: 'sizeof (Vertex)').
	* \param data The data pointer (first vertex).
	* \return The created vertex buffer handle.
	*/
	virtual vertexBufferHandle createVertexBuffer(int vertexCount, int vertexSize, void* data) = 0;

	/**
	 * \brief Binds given vertex buffer.
	 * \param handle The vertex buffer handle.
	 */
	virtual void useVertexBuffer(vertexBufferHandle handle) = 0;

	/**
	* \brief Releases vertex buffer.
	* \param handle The vertex buffer handle.
	*/
	virtual void destroyVertexBuffer(vertexBufferHandle handle) = 0;

	/**
	* \brief Creates new index buffer.
	* \param indexCount The index count.
	* \param is32bit Are indices 32 bit? (uint/int as data structure)
	* \param data The data pointer,
	* \return The created index buffer handle.
	*/
	virtual indexBufferHandle createIndexBuffer(int indexCount, bool is32bit, void* data) = 0;

	/**
	* \brief Binds given index buffer.
	* \param handle The index buffer handle.
	*/
	virtual void useIndexBuffer(indexBufferHandle handle) = 0;

	/**
	* \brief Releases index buffer.
	* \param handle The index buffer handle.
	*/
	virtual void destroyIndexBuffer(indexBufferHandle handle) = 0;

	/**
	 * \brief Creates new texture2d.
	 * \param width The new texture width.
	 * \param height The new texture height.
	 * \param mips The new texture mip map level count.
	 * \param format The new texture format.
	 * \param filtering The texture filtering type.
	 * \param data The new texture initial data.
	 * \return The created 2d texture.
	 */
	virtual texture2DHandle createTexture2D(uint width, uint height, int mips, Format::_enum format, Filtering::_enum filtering, void* data) = 0;

	/**
	 * \brief Applies texture to the current state.
	 * \param texture2d The texture handle.
	 * \param slot The texture target slot.
	 * \param shaderType The texture target shader type.
	 */
	virtual void applyTexture2D(texture2DHandle texture2d, int slot, ShaderType::_enum shaderType) = 0;
	
	// TODO: apply texture by name instead of slot and shaderType

	/**
	 * \brief Destroys given texture.
	 * \param texture2d The 2d texture which will be destroyed.
	 */
	virtual void destroyTexture2D(texture2DHandle texture2d) = 0;
	
	virtual renderBufferHandle createRenderBuffer(int textureCount, texture2DHandle* textures) = 0;
	virtual void cleanRenderBuffer(renderBufferHandle renderBuffer, float* color) = 0;
	virtual void useRenderBuffer(renderBufferHandle renderBuffer) = 0;
	virtual void resizeRenderBuffer(renderBufferHandle renderBuffer, uint width, uint height) = 0;
	virtual void destroyRenderBuffer(renderBufferHandle renderBuffer) = 0;

	/**
	 * \brief Loads shader file using given file name (relative to app exe file).
	 * \param fileName The file name (relative to app exe file).
	 * \return The loaded shader, or INVALID if failed (some weird errors).
	 */
	virtual shaderHandle loadShader(const char* fileName) = 0;

	/**
	 * \brief Applies the shader to the current state.
	 * \param handle The shader handle.
	 * \param passName The shader pass name.
	 */
	virtual void applyShader(shaderHandle handle, const char* passName) = 0;

	/**
	* \brief Applies the shader to the current state.
	* \param handle The shader handle.
	* \param passIndex The shader pass imdex.
	*/
	virtual void applyShader(shaderHandle handle, int passIndex) = 0;

	/**
	 * \brief Updates the shader buffer field value. 
	 * All changes are applied when shader is applied using (applyShader).
	 * \param handle The shader handle.
	 * \param bufferIndex Shader buffer index (starting at 0, we are not LUA).
	 * \param fieldIndex Shader buffer field index.
	 * \param value The new value pointer.
	 */
	virtual void updateShaderValue(shaderHandle handle, int bufferIndex, int fieldIndex, void* value) = 0;

	/**
	* \brief Updates the shader buffer field value.
	* All changes are applied when shader is applied using (applyShader).
	* \param handle The shader handle.
	* \param bufferName Shader buffer name.
	* \param fieldName Shader buffer field name.
	* \param value The new value pointer.
	*/
	virtual void updateShaderValue(shaderHandle handle, const char* bufferName, const char* fieldName, void* value) = 0;

	/**
	 * \brief Destroys shader.
	 * \param handle The shader handle which will be destroyed.
	 */
	virtual void destroyShader(shaderHandle handle) = 0;

	/**
	 * \brief Draws vertex triangle list.
	 * \param vertexCount The vertex count.
	 */
	virtual void draw(int vertexCount) = 0;

	/**
	 * \brief Draws indexed triangle list.
	 * \param indexCount The index count.
	 */
	virtual void drawIndexed(int indexCount) = 0;

	/**
	 * \brief Resizes internal render buffers.
	 * \param width The width of render output.
	 * \param height The height of render output 
	 */
	virtual void resize(uint width, uint height) = 0;

	/**
	 * \brief Begins new frame, resets everything (eg.: sets default renderBuffers).
	 */
	virtual void beginFrame() = 0;

	/**
	 * \brief Presents the current frame
	 * \param vSync Should use v-blank synchronization?
	 */
	virtual void frame(bool vSync) = 0;

	/**
	 * \brief Shutdowns the renderer.
	 */
	virtual void shutdown()
	{
		delete this;
	}

public:
	/**
	 * \brief Initializes new renderer type.
	 * This can be called only once!
	 * \param rendererType The target renderer type.
	 * \param windowHandle The output window handle (HWND).
	 * \param multithreaded Should run as multithreaded?
	 * \return The current renderer instance pointer (same as Renderer::current).
	 */
	static Renderer* initialize(RendererType::_enum rendererType, void* windowHandle, bool multithreaded);

public:
	/**
	 * \brief The current renderer.
	 */
	static Renderer* current;

};

#endif // RENDERER_H
