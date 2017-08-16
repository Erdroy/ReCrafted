// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef D3D11RENDERER_H
#define D3D11RENDERER_H

// incldues
#include "../Renderer.h"

/*
 * Warning: Do not expose any of D3D11.h objects!
 * You will be dead if you do so.
 */

class D3D11Renderer : public Renderer
{
	friend struct D3D11ShaderProgram;

private:
	void initializeDevice(void* windowHandle) const;

	void clean() const;

public:
	D3D11Renderer() {}

protected:
	~D3D11Renderer() override
	{
		clean();
	}

protected:
	void initialize(void* windowHandle, bool multithreaded) override;

public:
	/**
	* \brief Creates new vertex buffer.
	* \param vertexCount The vertex count.
	* \param vertexSize The vertex size (eg.: 'sizeof (Vertex)').
	* \param data The data pointer (first vertex).
	* \return The created vertex buffer handle.
	*/
	vertexBufferHandle createVertexBuffer(int vertexCount, int vertexSize, void* data) override;

	/**
	* \brief Binds given vertex buffer.
	* \param handle The vertex buffer handle.
	*/
	void useVertexBuffer(vertexBufferHandle handle) override;

	/**
	* \brief Releases vertex buffer.
	* \param handle The vertex buffer handle.
	*/
	void destroyVertexBuffer(vertexBufferHandle handle) override;

	/**
	* \brief Creates new index buffer.
	* \param indexCount The index count.
	* \param is32bit Are indices 32 bit? (uint/int as data structure)
	* \param data The data pointer,
	* \return The created index buffer handle.
	*/
	indexBufferHandle createIndexBuffer(int indexCount, bool is32bit, void* data) override;

	/**
	* \brief Binds given index buffer.
	* \param handle The index buffer handle.
	*/
	void useIndexBuffer(indexBufferHandle handle) override;

	/**
	* \brief Releases index buffer.
	* \param handle The index buffer handle.
	*/
	void destroyIndexBuffer(indexBufferHandle handle) override;

	/**
	* \brief Creates new texture2d.
	* \param width The new texture width.
	* \param height The new texture height.
	* \param mips The new texture mip map level count.
	* \param format The new texture format.
	* \param data The new texture initial data.
	* \return The created 2d texture.
	*/
	texture2DHandle createTexture2D(uint width, uint height, int mips, Format::_enum format, void* data) override;

	/**
	* \brief Applies texture to the current state.
	* \param texture2d The texture handle.
	* \param slot The texture target slot.
	* \param shaderType The texture target shader type.
	*/
	void applyTexture2D(texture2DHandle texture2d, int slot, ShaderType::_enum shaderType) override;

	/**
	* \brief Destroys given texture.
	* \param texture2d The 2d texture which will be destroyed.
	*/
	void destroyTexture2D(texture2DHandle texture2d) override;

	renderBufferHandle createRenderBuffer(int textureCount, texture2DHandle* textures) override;
	void cleanRenderBuffer(renderBufferHandle renderBuffer, float* color) override;
	void useRenderBuffer(renderBufferHandle renderBuffer) override;
	void resizeRenderBuffer(renderBufferHandle renderBuffer, uint width, uint height) override;
	void destroyRenderBuffer(renderBufferHandle renderBuffer) override;

	/**
	* \brief Loads shader file using given file name (relative to app exe file).
	* \param fileName The file name (relative to app exe file).
	* \return The loaded shader, or INVALID if failed (some weird errors).
	*/
	shaderHandle loadShader(const char* fileName) override;

	/**
	* \brief Applies the shader to the current state.
	* \param handle The shader handle.
	* \param passName The shader pass name.
	*/
	void applyShader(shaderHandle handle, const char* passName) override;

	/**
	* \brief Applies the shader to the current state.
	* \param handle The shader handle.
	* \param passIndex The shader pass imdex.
	*/
	void applyShader(shaderHandle handle, int passIndex) override;

	/**
	* \brief Updates the shader buffer field value.
	* All changes are applied when shader is applied using (applyShader).
	* \param handle The shader handle.
	* \param bufferIndex Shader buffer index (starting at 0, we are not LUA).
	* \param fieldIndex Shader buffer field index.
	* \param value The new value pointer.
	*/
	void updateShaderValue(shaderHandle handle, int bufferIndex, int fieldIndex, void* value) override;

	/**
	* \brief Updates the shader buffer field value.
	* All changes are applied when shader is applied using (applyShader).
	* \param handle The shader handle.
	* \param bufferName Shader buffer name.
	* \param fieldName Shader buffer field name.
	* \param value The new value pointer.
	*/
	void updateShaderValue(shaderHandle handle, const char* bufferName, const char* fieldName, void* value) override;

	/**
	* \brief Destroys shader.
	* \param handle The shader handle which will be destroyed.
	*/
	void destroyShader(shaderHandle handle) override;

	/**
	* \brief Draws vertex triangle list.
	* \param vertexCount The vertex count.
	*/
	void draw(int vertexCount) override;

	/**
	* \brief Draws indexed triangle list.
	* \param indexCount The index count.
	*/
	void drawIndexed(int indexCount) override;

	/**
	* \brief Resizes internal render buffers.
	* \param width The width of render output.
	* \param height The height of render output
	*/
	void resize(uint width, uint height) override;

	/**
	* \brief Begins new frame, resets everything (eg.: sets default renderBuffers).
	*/
	void beginFrame() override;

	/**
	* \brief Presents the current frame
	* \param vSync Should use v-blank synchronization?
	*/
	void frame(bool vSync) override;

	/**
	* \brief Shutdowns the renderer.
	*/
	void shutdown() override;

public:
	static void* getDevice();
	static void* getDeviceContext();
};

#endif // D3D11RENDERER_H
