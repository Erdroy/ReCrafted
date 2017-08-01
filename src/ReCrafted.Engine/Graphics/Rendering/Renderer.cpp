// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Renderer.h"

#include "D3D11/D3D11Renderer.h"

Renderer* Renderer::current;

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

	current = renderer;
	return renderer;
}
