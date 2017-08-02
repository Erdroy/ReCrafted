// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef RENDERER_H
#define RENDERER_H

// includes
#include "Utils/Types.h"

struct RendererType
{
	enum _enum
	{
		DirectX11,
		Vulkan
	};
};

#define CREATE_HANDLE(name) \
	struct name##Handle\
	{\
	public:\
		uint m_idx = 0;\
		name##Handle () {} \
		name##Handle (uint idx) : m_idx(idx) {} \
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
	* \param size The data size (vertex size * count).
	* \param data The data pointer.
	* \return The created vertex buffer handle.
	*/
	virtual vertexBufferHandle createVertexBuffer(int size, void* data) = 0;
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
	* \brief Releases index buffer.
	* \param handle The index buffer handle.
	*/
	virtual void destroyIndexBuffer(indexBufferHandle handle) = 0;

	virtual texture2DHandle createTexture2D(uint width, uint height, int mips, int format, void* data) = 0;
	virtual void destroyTexture2D(texture2DHandle texture2d) = 0;
	
	virtual renderBufferHandle createRenderBuffer(int textureCount, texture2DHandle* textures) = 0;
	virtual void cleanRenderBuffer(renderBufferHandle renderBuffer, float* color) = 0;
	virtual void useRenderBuffer(renderBufferHandle renderBuffer) = 0;
	virtual void destroyRenderBuffer(renderBufferHandle renderBuffer) = 0;

	virtual void resize(uint width, uint height) = 0;
	virtual void beginFrame() = 0;
	virtual void frame(bool vSync) = 0;

	void shutdown() const
	{
		delete this;
	}

public:
	static Renderer* initialize(RendererType::_enum rendererType, void* windowHandle, bool multithreaded);

public:
	static Renderer* current;

};

#endif // RENDERER_H
