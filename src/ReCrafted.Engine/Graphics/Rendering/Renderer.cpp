// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Renderer.h"

#include "D3D11/D3D11Renderer.h"

Renderer* Renderer::current;

VertexElem::VertexElem(VertexAttrib::_enum attrib, int count, VertexAttribType::_enum attribType):
	attrib(attrib),
	count(count),
	attribType(attribType)
{
}

Renderer* Renderer::initialize(RendererType::_enum rendererType, void* windowHandle, bool multithreaded)
{
	Renderer* renderer;

	switch(rendererType)
	{
	case RendererType::DirectX11:
		renderer = new D3D11Renderer();
		break;
	case RendererType::Vulkan:
		return nullptr; // TODO: Vulkan Renderer
	default:
		return nullptr;
	}
	renderer->initialize(windowHandle, multithreaded);

	VertexDesc desc = {};
	desc.add(VertexAttrib::Position, 3, VertexAttribType::Float);
	desc.add(VertexAttrib::TexCoord0, 4, VertexAttribType::Float);

	renderer->createVertexBuffer(1, 4 * 4, desc, nullptr);

	current = renderer;
	return renderer;
}
