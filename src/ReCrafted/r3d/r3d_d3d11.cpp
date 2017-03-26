// r3d © 2016-2017 Damian 'Erdroy' Korczowski

#include "r3d_d3d11.h"
#include "r3d_commandlist.h"
#include "r3d_headers.h"

#include <windows.h>
#include <d3d11_1.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")

bool m_vsync = true;

ID3D11Device* m_device = nullptr;
ID3D11DeviceContext* m_deviceContext = nullptr;
IDXGISwapChain* m_swapchain = nullptr;

ID3D11RenderTargetView* m_rendertarget_final = nullptr;
ID3D11DepthStencilView* m_depthStencilView = nullptr;

void r3d_d3d11::init(void* window_handle, uint16_t width, uint16_t height)
{
	DXGI_SWAP_CHAIN_DESC swapchain_desc = {};
	swapchain_desc.BufferCount = 1;
	swapchain_desc.BufferDesc.Width = width;
	swapchain_desc.BufferDesc.Height = height;
	swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchain_desc.OutputWindow = static_cast<HWND>(window_handle);
	swapchain_desc.SampleDesc.Count = 1;
	swapchain_desc.Windowed = true;

	// feature levels
	D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_1 };

	// create device, device context and swapchain
	auto hr = D3D11CreateDeviceAndSwapChain(nullptr, 
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 
		0, 
		levels, 
		1, 
		D3D11_SDK_VERSION,
		&swapchain_desc,
		&m_swapchain, 
		&m_device,
		nullptr, 
		&m_deviceContext
	);

	if (FAILED(hr))
		throw "D3D11CreateDeviceAndSwapChain failed";

	// make backbuffer
	ID3D11Texture2D* backBufferPtr;
	hr = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBufferPtr));

	if (FAILED(hr))
		throw "m_swapchain->GetBuffer failed";

	hr = m_device->CreateRenderTargetView(backBufferPtr, nullptr, &m_rendertarget_final);

	if (FAILED(hr))
		throw "m_device->CreateRenderTargetView failed";

	backBufferPtr->Release();

	// make depth stencil buffer
	D3D11_TEXTURE2D_DESC depthBufferDesc = {};

	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
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
		throw "m_device->CreateTexture2D failed";

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = m_device->CreateDepthStencilView(depthBuffer, &depthStencilViewDesc, &m_depthStencilView);

	if (FAILED(hr))
		throw "m_device->CreateDepthStencilView failed";


}

void r3d_d3d11::resize(uint16_t width, uint16_t height)
{
	// TODO: resize all built-in render targets
}

void r3d_d3d11::destroy()
{
	// release all resources
	m_swapchain->Release();
	m_deviceContext->Release();
	m_device->Release();
}

void r3d_d3d11::execute_commandlist(r3d_commandlist* cmdListPtr)
{
	_ASSERT(m_device != nullptr);
	_ASSERT(m_deviceContext != nullptr);
	_ASSERT(m_swapchain != nullptr);

	auto cmdlist = cmdListPtr;

	ID3D11RenderTargetView* views[] = {m_rendertarget_final};

	// read command list
	while (!cmdlist->is_read_end())
	{
		// read command header
		auto header = static_cast<r3d_cmdlist_header::Enum>(cmdlist->read_uint32());
		
		// execute command
		switch(header)
		{
		case r3d_cmdlist_header::beginframe:
			m_deviceContext->OMSetRenderTargets(1, views, m_depthStencilView);
			continue;
		case r3d_cmdlist_header::endframe:
			m_swapchain->Present(m_vsync ? 1 : 0, 0);
			continue;


		case r3d_cmdlist_header::vsync:
			m_vsync = cmdlist->read_bool();
			continue;

		case r3d_cmdlist_header::clearcolor:
		{
			auto color = cmdlist->read_ptr(); // color ptr
			m_deviceContext->ClearRenderTargetView(m_rendertarget_final, static_cast<float*>(color));
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