// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "D3D11Renderer.h"

#include "D3D11.h"
#include "D3D11RenderBuffer.h"
#include "D3D11ShaderProgram.h"
#include "Core/Logger.h"

#include "../Config.h"
#include "../Utils.h"

#pragma comment (lib, "DXGI.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define DXCALL(x) if(FAILED(##x##))

#define OBJECT_RELEASE_DEF(name, handleType, maxV) \
	inline void name##_release(handleType handle) { \
		if(handle.m_idx >= maxV)\
			return;\
		auto val = m_##name##Array[handle.m_idx]; \
		if(val) val->Release(); \
		name##_free(handle);\
	}

struct VertexBufferInfo
{
public:
	uint stride;
	uint length;
};

struct IndexBufferInfo
{
public:
	bool is32bit;

};

struct ShaderInfo
{
	
};


OBJECT_ARRAY(ID3D11Buffer*, vertexBuffer, vertexBufferHandle, RENDERER_MAX_VERTEX_BUFFERS)
OBJECT_INFO_ARRAY(VertexBufferInfo, vertexBufferInfo, vertexBufferHandle, RENDERER_MAX_INDEX_BUFFERS)

OBJECT_ARRAY(ID3D11Buffer*, indexBuffer, indexBufferHandle, RENDERER_MAX_INDEX_BUFFERS)
OBJECT_INFO_ARRAY(IndexBufferInfo, indexBufferInfo, indexBufferHandle, RENDERER_MAX_INDEX_BUFFERS)

OBJECT_ARRAY(ID3D11RenderBuffer*, renderBuffer, renderBufferHandle, RENDERER_MAX_RENDER_BUFFERS)

OBJECT_ARRAY(D3D11ShaderProgram*, shaderProgram, shaderHandle, RENDERER_MAX_SHADERS)

ID3D11Device* m_device = nullptr;
ID3D11DeviceContext* m_deviceContext = nullptr;
IDXGISwapChain* m_swapchain = nullptr;

ID3D11RenderTargetView* m_renderTarget = nullptr;

bool m_multithreaded = false;

const char* AttribToSemantic(VertexAttrib::_enum attrib)
{
	switch (attrib) {
	case VertexAttrib::Position:
		return "POSITION";
	case VertexAttrib::Normal:
		return "NORMAL";
	case VertexAttrib::Tangent:
		return "TANGENT";
	case VertexAttrib::BiTangent:
		return "BITANGENT";
	case VertexAttrib::Color0:
		return "COLOR0";
	case VertexAttrib::Color1:
		return "COLOR1";
	case VertexAttrib::Color2:
		return "COLOR2";
	case VertexAttrib::Color3:
		return "COLOR3";
	case VertexAttrib::TexCoord0:
		return "TEXCOORD0";
	case VertexAttrib::TexCoord1:
		return "TEXCOORD1";
	case VertexAttrib::TexCoord2:
		return "TEXCOORD2";
	case VertexAttrib::TexCoord3:
		return "TEXCOORD3";
	case VertexAttrib::TexCoord4:
		return "TEXCOORD4";
	case VertexAttrib::TexCoord5:
		return "TEXCOORD5";
	case VertexAttrib::TexCoord6:
		return "TEXCOORD6";
	case VertexAttrib::TexCoord7:
		return "TEXCOORD7";
	case VertexAttrib::Count:
	default:
		return "UNKNOWN";
	}
}

DXGI_FORMAT AttribTypeToFormat(VertexAttribType::_enum attribType, int count)
{
	switch (attribType)
	{
	case VertexAttribType::UInt8:
		switch (count)
		{
		case 1:
			return DXGI_FORMAT_R8_UINT;
		case 2:
			return DXGI_FORMAT_R8G8_UINT;
		case 4:
			return DXGI_FORMAT_R8G8B8A8_UINT;

		default:
			return DXGI_FORMAT_R8G8B8A8_UINT;
		}
	case VertexAttribType::Int16: 
		switch (count)
		{
		case 1:
			return DXGI_FORMAT_R16_UINT;
		case 2:
			return DXGI_FORMAT_R16G16_UINT;
		case 4:
			return DXGI_FORMAT_R16G16B16A16_UINT;

		default:
			return DXGI_FORMAT_R16G16B16A16_UINT;
		}
	case VertexAttribType::Half:
		switch (count)
		{
		case 1:
			return DXGI_FORMAT_R16_FLOAT;
		case 2:
			return DXGI_FORMAT_R16G16_FLOAT;
		case 4:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;

		default:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;
		}
	case VertexAttribType::Float: 
		switch (count)
		{
		case 1:
			return DXGI_FORMAT_R32_FLOAT;
		case 2:
			return DXGI_FORMAT_R32G32_FLOAT;
		case 4:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;

		default:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
	case VertexAttribType::Count:
	default:
		return DXGI_FORMAT_UNKNOWN;
	}
}

void D3D11Renderer::initializeDevice(void* windowHandle) const
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

	// set primitive topology
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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

	// TODO: create default render buffer
}

vertexBufferHandle D3D11Renderer::createVertexBuffer(int vertexCount, int vertexSize, void* data)
{
	auto buffer = vertexBuffer_alloc();

	if(IS_INVALID(buffer))
		return{};

	auto size = vertexSize * vertexCount;

	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = size;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA subresource_data;
	subresource_data.pSysMem = data;
	subresource_data.SysMemPitch = 0;
	subresource_data.SysMemSlicePitch = 0;

	ID3D11Buffer* bufferPtr = nullptr;
	DXCALL(m_device->CreateBuffer(&desc, data ? &subresource_data : nullptr, &bufferPtr))
	{
		return{};
	}

	VertexBufferInfo info;
	info.length = vertexCount;
	info.stride = vertexSize;
	vertexBufferInfo_set(buffer, info);

	// set pointer
	vertexBuffer_set(buffer, bufferPtr);

	/*D3D11_INPUT_ELEMENT_DESC inputElements[16] = {};

	for(auto i = 0u; i < vertexDesc.m_count; i ++)
	{
		D3D11_INPUT_ELEMENT_DESC inputElement = {};

		auto element = vertexDesc.m_elemets[i];

		inputElement.SemanticName = AttribToSemantic(element.attrib);
		inputElement.Format = AttribTypeToFormat(element.attribType, element.count);
		inputElement.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		inputElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		inputElements[i] = inputElement;
	}*/

	return buffer;
}

void D3D11Renderer::useVertexBuffer(vertexBufferHandle handle)
{
	if(IS_INVALID(handle))
		return;

	auto buffer = vertexBuffer_get(handle);

	// get info (we need stride)
	auto info = vertexBufferInfo_get(handle);

	// offset should be always zero, 
	// we don't need this actually
	uint offset = 0;

	ID3D11Buffer* buffers[] = { buffer };
	m_deviceContext->IASetVertexBuffers(0, 1, buffers, &info->stride, &offset);
}

void D3D11Renderer::destroyVertexBuffer(vertexBufferHandle handle)
{
	if (IS_INVALID(handle))
		return;

	vertexBuffer_release(handle);
}

indexBufferHandle D3D11Renderer::createIndexBuffer(int indexCount, bool is32bit, void* data)
{
	auto buffer = indexBuffer_alloc();

	if (IS_INVALID(buffer))
		return{};

	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = indexCount * is32bit ? 4 : 2;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA subresource_data;
	subresource_data.pSysMem = data;
	subresource_data.SysMemPitch = 0;
	subresource_data.SysMemSlicePitch = 0;

	ID3D11Buffer* bufferPtr = nullptr;
	DXCALL(m_device->CreateBuffer(&desc, data ? &subresource_data : nullptr, &bufferPtr))
	{
		return{};
	}

	// set pointer
	indexBuffer_set(buffer, bufferPtr);

	// set info
	IndexBufferInfo info;
	info.is32bit = is32bit;
	indexBufferInfo_set(buffer, info);

	return buffer;
}

void D3D11Renderer::useIndexBuffer(indexBufferHandle handle)
{
	if (IS_INVALID(handle))
		return;

	// get info
	auto info = indexBufferInfo_get(handle);

	// set index buffer
	m_deviceContext->IASetIndexBuffer(indexBuffer_get(handle), info->is32bit ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, 0);
}

void D3D11Renderer::destroyIndexBuffer(indexBufferHandle handle)
{
	if (IS_INVALID(handle))
		return;

	indexBuffer_release(handle);
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

void D3D11Renderer::resizeRenderBuffer(renderBufferHandle renderBuffer, uint width, uint height)
{
}

void D3D11Renderer::destroyRenderBuffer(renderBufferHandle renderBuffer)
{
}

shaderHandle D3D11Renderer::loadShader(const char* fileName)
{
	if (!Platform::fileExists(fileName))
		return {};

	auto shaderHandle = shaderProgram_alloc();

	auto shaderProgram = LoadShader(fileName);

	shaderProgram_set(shaderHandle, shaderProgram);

	
	return{};
}

void D3D11Renderer::destroyShader(shaderHandle handle)
{
}

void D3D11Renderer::draw(int vertexCount)
{
	m_deviceContext->Draw(vertexCount, 0);
}

void D3D11Renderer::drawIndexed(int indexCount)
{
	m_deviceContext->DrawIndexed(indexCount, 0u, 0u);
}

void D3D11Renderer::resize(uint width, uint height)
{
	// TODO: resize all internal RTs
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
