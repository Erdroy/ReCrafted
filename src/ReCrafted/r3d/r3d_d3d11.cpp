// r3d © 2016-2017 Damian 'Erdroy' Korczowski

#include "r3d_d3d11.h"
#include "r3d_commandlist.h"
#include "r3d_headers.h"

#include <windows.h>
#include <d3d11_1.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")

bool m_vsync = true;

class context
{
public:
	IDXGISwapChain* swapchain = nullptr;
	ID3D11RenderTargetView* backbuffer = nullptr;
	ID3D11DepthStencilView* depthstencil = nullptr;




public:
	void release() const
	{
		backbuffer->Release();
		depthstencil->Release();
		swapchain->Release();
	}
};

context* m_contexts[32 /* same as MAX_WINDOWS */] = {};
context* m_currentContext = nullptr;

ID3D11Device* m_device = nullptr;
ID3D11DeviceContext* m_deviceContext = nullptr;

void r3d_d3d11::init()
{
	// feature levels
	D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_1 };
	auto hr = D3D11CreateDevice(nullptr,
		D3D_DRIVER_TYPE_HARDWARE, 
		nullptr, 
		0, 
		levels, 
		1, 
		D3D11_SDK_VERSION, 
		&m_device,
		nullptr, 
		&m_deviceContext
	);

	if (FAILED(hr))
		throw "D3D11CreateDevice failed";
}

void r3d_d3d11::create_context(r3d_window_handle* window)
{
	auto windowHandle = HWND(r3d::window_getnativeptr(window));

	m_contexts[window->idx] = new context;
	m_currentContext = m_contexts[window->idx];

	RECT rect;
	GetClientRect(windowHandle, &rect);

	IDXGIDevice* device;
	auto hr = m_device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&device));

	if (FAILED(hr))
		throw "create_context QueryInterface failed";

	IDXGIAdapter* adapter;
	hr = device->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&adapter));

	if (FAILED(hr))
		throw "create_context GetParent1 failed";

	IDXGIFactory* factory;
	hr = adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));

	if (FAILED(hr))
		throw "create_context GetParent2 failed";

	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferCount = 1;
	scd.BufferDesc.Width = rect.right - rect.left;
	scd.BufferDesc.Height = rect.bottom - rect.top;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.OutputWindow = windowHandle;
	scd.Windowed = TRUE;

	hr = factory->CreateSwapChain(device, &scd, &m_currentContext->swapchain);

	if (FAILED(hr))
		throw "create_context CreateSwapChain failed";

	// create back buffer
	ID3D11Texture2D* backBufferPtr;
	hr = m_currentContext->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBufferPtr));

	if (FAILED(hr))
		throw "m_swapchain->GetBuffer failed";

	hr = m_device->CreateRenderTargetView(backBufferPtr, nullptr, &m_currentContext->backbuffer);

	if (FAILED(hr))
		throw "m_device->CreateRenderTargetView failed";

	backBufferPtr->Release();

	// create depth buffer
	D3D11_TEXTURE2D_DESC depthBufferDesc = {};
	depthBufferDesc.Width = rect.right - rect.left;
	depthBufferDesc.Height = rect.bottom - rect.top;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	ID3D11Texture2D* depthBuffer = nullptr;

	hr = m_device->CreateTexture2D(&depthBufferDesc, nullptr, &depthBuffer);

	if (FAILED(hr))
		throw "create_context CreateTexture2D failed";

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = m_device->CreateDepthStencilView(depthBuffer, &depthStencilViewDesc, &m_currentContext->depthstencil);

	if (FAILED(hr))
		throw "create_context CreateDepthStencilView failed";
}

void r3d_d3d11::make_current(r3d_window_handle* window)
{
	m_currentContext = m_contexts[window->idx];
}

void r3d_d3d11::resize(uint16_t width, uint16_t height)
{
	// TODO: resize all built-in render targets
}

void r3d_d3d11::destroy()
{
	// release all resources
	for(auto i = 0; i < 32; i ++)
	{
		if(m_contexts[i] != nullptr)
		{
			m_contexts[i]->release();
			delete m_contexts[i];
		}
	}

	m_deviceContext->Release();
	m_device->Release();
}

void r3d_d3d11::execute_commandlist(r3d_commandlist* cmdListPtr)
{
	_ASSERT(m_device != nullptr);
	_ASSERT(m_deviceContext != nullptr);
	_ASSERT(m_currentContext != nullptr);

	auto cmdlist = cmdListPtr;

	ID3D11RenderTargetView* views[] = { m_currentContext->backbuffer };

	// read command list
	while (!cmdlist->is_read_end())
	{
		// read command header
		auto header = static_cast<r3d_cmdlist_header::Enum>(cmdlist->read_uint32());
		
		// execute command
		switch(header)
		{
		case r3d_cmdlist_header::beginframe:
			m_deviceContext->OMSetRenderTargets(1, views, m_currentContext->depthstencil);
			continue;
		case r3d_cmdlist_header::endframe:
			m_currentContext->swapchain->Present(m_vsync ? 1 : 0, 0);
			continue;


		case r3d_cmdlist_header::vsync:
			m_vsync = cmdlist->read_bool();
			continue;

		case r3d_cmdlist_header::clearcolor:
		{
			auto color = cmdlist->read_ptr(); // color ptr
			m_deviceContext->ClearRenderTargetView(m_currentContext->backbuffer, static_cast<float*>(color));
			continue;
		}
		case r3d_cmdlist_header::cleardepth:
			continue;

		case r3d_cmdlist_header::count:
		default: 
			throw;
		}
	}
}