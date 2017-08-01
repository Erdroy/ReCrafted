// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "D3D11Renderer.h"

#include "D3D11.h"
#include "Core/Logger.h"

#pragma comment (lib, "DXGI.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define DXCALL(x) if(FAILED(##x##))

ID3D11Device* m_device = nullptr;
ID3D11DeviceContext* m_deviceContext = nullptr;
IDXGISwapChain* m_swapchain = nullptr;

ID3D11RenderTargetView* m_renderTarget = nullptr;

bool m_multithreaded = false;

void D3D11Renderer::initializeDevice(void* windowHandle)
{
	D3D_FEATURE_LEVEL level;
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

#ifdef _DEBUG
	auto flags = D3D11_CREATE_DEVICE_DEBUG;
#else
	auto flags = 0;
#endif

	// get window size
	auto hWnd = static_cast<HWND>(windowHandle);
	RECT clRect;
	GetClientRect(hWnd, &clRect);

	auto width = clRect.right - clRect.left;
	auto height = clRect.bottom - clRect.top;

	DXGI_SWAP_CHAIN_DESC swapchainDesc = {};
	swapchainDesc.BufferCount = 1;
	swapchainDesc.BufferDesc.Width = static_cast<UINT>(width);
	swapchainDesc.BufferDesc.Height = static_cast<UINT>(height);
	swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.SampleDesc.Quality = 0;
	swapchainDesc.OutputWindow = hWnd;
	swapchainDesc.Windowed = TRUE;

	// initialize device
	DXCALL(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		flags,
		featureLevels,
		ARRAYSIZE(featureLevels),
		D3D11_SDK_VERSION,
		&swapchainDesc,
		&m_swapchain,
		&m_device,
		&level,
		&m_deviceContext))
	{
		Logger::write("Failed to create ID3D11Device", LogLevel::Fatal);
		return;
	}

	CComPtr<ID3D11Texture2D> backBufferPtr;
	DXCALL(m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBufferPtr)))
	{
		Logger::write("Failed to create ID3D11Texture2D (m_swapchain->GetBuffer...)", LogLevel::Fatal);
		return;
	}

	DXCALL(m_device->CreateRenderTargetView(backBufferPtr, nullptr, &m_renderTarget))
	{
		Logger::write("Failed to create RenderTargetView", LogLevel::Fatal);
		return;
	}

	backBufferPtr.Release();
}

void D3D11Renderer::clean() const
{
	m_renderTarget->Release();
	m_swapchain->Release();
	m_deviceContext->Release();
	m_device->Release();
}

void D3D11Renderer::initialize(void* windowHandle, bool multithreaded)
{
	m_multithreaded = multithreaded;

	initializeDevice(windowHandle);

	// create threads if this is multithreaded
	//if (m_multithreaded)
	//	initializeThreads();
}

vertexBufferHandle D3D11Renderer::createVertexBuffer(int vertexCount, int strip, int length, void* data)
{
	return{};
}

void D3D11Renderer::destroyVertexBuffer(vertexBufferHandle handle)
{
}

indexBufferHandle D3D11Renderer::createIndexBuffer(int indexCount, bool is32bit, void* data)
{
	return{};
}

void D3D11Renderer::destroyIndexBuffer(indexBufferHandle handle)
{
}

texture2DHandle D3D11Renderer::createTexture2D(uint width, uint height, int mips, int format, void* data)
{
	return{};
}

void D3D11Renderer::destroyTexture2D(texture2DHandle texture2d)
{
}

renderBufferHandle D3D11Renderer::createRenderBuffer(int textureCount, texture2DHandle* textures)
{
	return{};
}

void D3D11Renderer::cleanRenderBuffer(renderBufferHandle renderBuffer, float* color)
{
}

void D3D11Renderer::useRenderBuffer(renderBufferHandle renderBuffer)
{
}

void D3D11Renderer::destroyRenderBuffer(renderBufferHandle renderBuffer)
{
}

void D3D11Renderer::resize(uint width, uint height)
{
	// TODO: resize all
}

void D3D11Renderer::beginFrame()
{
	// set default RTs
	ID3D11RenderTargetView* rts[] = {
		m_renderTarget
	};
	m_deviceContext->OMSetRenderTargets(1, rts, nullptr);

	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	// clean
	m_deviceContext->ClearRenderTargetView(m_renderTarget, clearColor);
}

void D3D11Renderer::frame(bool vSync)
{
	m_swapchain->Present(vSync ? 1 : 0, 0);
}
