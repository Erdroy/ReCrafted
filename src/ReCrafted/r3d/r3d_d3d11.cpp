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

void r3d_d3d11::init(void* window_handle)
{
	DXGI_SWAP_CHAIN_DESC swapchain_desc = {};
	swapchain_desc.BufferCount = 1;
	swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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

	if(FAILED(hr))
		throw "D3D11CreateDeviceAndSwapChain failed";
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

	// read command list
	while (!cmdlist->is_read_end())
	{
		// read command header
		auto header = static_cast<r3d_cmdlist_header::Enum>(cmdlist->read_uint32());
		
		// execute command
		switch(header)
		{
		case r3d_cmdlist_header::vsync:
			m_vsync = cmdlist->read_bool();
			continue;
		case r3d_cmdlist_header::count:
		default: 
			throw;
		}
	}
}