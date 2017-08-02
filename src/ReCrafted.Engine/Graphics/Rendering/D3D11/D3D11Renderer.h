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
private:
	void initializeDevice(void* windowHandle);

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
	vertexBufferHandle createVertexBuffer(int size, void* data) override;
	void destroyVertexBuffer(vertexBufferHandle handle) override;

	indexBufferHandle createIndexBuffer(int indexCount, bool is32bit, void* data) override;
	void destroyIndexBuffer(indexBufferHandle handle) override;

	texture2DHandle createTexture2D(uint width, uint height, int mips, int format, void* data) override;
	void destroyTexture2D(texture2DHandle texture2d) override;

	renderBufferHandle createRenderBuffer(int textureCount, texture2DHandle* textures) override;
	void cleanRenderBuffer(renderBufferHandle renderBuffer, float* color) override;
	void useRenderBuffer(renderBufferHandle renderBuffer) override;
	void destroyRenderBuffer(renderBufferHandle renderBuffer) override;

	void resize(uint width, uint height) override;
	void beginFrame() override;
	void frame(bool vSync) override;
};

#endif // D3D11RENDERER_H
