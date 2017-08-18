// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "D3D11RenderBuffer.h"
#include "D3D11.h"

ID3D11RenderBuffer* CreateRenderBuffer(uint textureCount, ID3D11Texture2D* textures[16])
{
	auto rb = new ID3D11RenderBuffer;

	// TODO: process textures

	return rb;
}
