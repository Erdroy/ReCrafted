// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "D3D11ShaderProgram.h"

#include "../../../../ReCrafted.ShaderCompiler/Compiler/ShaderMeta.h"
#include "../../../../ReCrafted.ShaderCompiler/Compiler/File.h"

#include "D3D11Renderer.h"

#define DXCALL(x) if(FAILED(##x##))

void* m_lastInputLayout = nullptr;

struct InputLayout
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
	CComPtr<ID3D11InputLayout> inputLayout;
};

std::vector<InputLayout> input_layouts = {};

typedef enum // from `hlslcc.h`
{
	INVALID_SHADER = -1,
	PIXEL_SHADER,
	VERTEX_SHADER,
	GEOMETRY_SHADER,
	HULL_SHADER,
	DOMAIN_SHADER,
	COMPUTE_SHADER,
} SHADER_TYPE;

CComPtr<ID3D11SamplerState> m_samplerState_pc;
CComPtr<ID3D11SamplerState> m_samplerState_pw;
CComPtr<ID3D11SamplerState> m_samplerState_pm;

CComPtr<ID3D11SamplerState> m_samplerState_lc;
CComPtr<ID3D11SamplerState> m_samplerState_lw;
CComPtr<ID3D11SamplerState> m_samplerState_lm;

void InitializeSamplerStates()
{
	auto device = static_cast<ID3D11Device*>(D3D11Renderer::getDevice());

	CD3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	DXCALL(device->CreateSamplerState(&samplerDesc, &m_samplerState_pc)) throw;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	DXCALL(device->CreateSamplerState(&samplerDesc, &m_samplerState_pw)) throw;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	DXCALL(device->CreateSamplerState(&samplerDesc, &m_samplerState_pm)) throw;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	DXCALL(device->CreateSamplerState(&samplerDesc, &m_samplerState_lc)) throw;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	DXCALL(device->CreateSamplerState(&samplerDesc, &m_samplerState_lw)) throw;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	DXCALL(device->CreateSamplerState(&samplerDesc, &m_samplerState_lm)) throw;
}

void InitializeAnisoSamplerStates(int level)
{
}

uint8_t TypeSizeFromString(const char* typeName)
{
	// int
	if (strcmp(typeName, "int") == 0)
	{
		return 4u;
	}

	// uint
	if (strcmp(typeName, "uint") == 0)
	{
		return 4u;
	}

	// dword
	if (strcmp(typeName, "dword") == 0)
	{
		return 4u;
	}

	// float
	if (strcmp(typeName, "float") == 0)
	{
		return 4u;
	}

	// float2
	if (strcmp(typeName, "float2") == 0)
	{
		return 4u * 2;
	}

	// float3
	if (strcmp(typeName, "float3") == 0)
	{
		return 4u * 3;
	}

	// float4
	if (strcmp(typeName, "float4") == 0)
	{
		return 4u * 4;
	}

	// float2x2
	if (strcmp(typeName, "float2x2") == 0)
	{
		return 4u * 2 * 2;
	}

	// float3x2
	if (strcmp(typeName, "float3x2") == 0)
	{
		return 4u * 3 * 2;
	}

	// float4x2
	if (strcmp(typeName, "float4x2") == 0)
	{
		return 4u * 4 * 2;
	}

	// float2x3
	if (strcmp(typeName, "float2x3") == 0)
	{
		return 4u * 2 * 3;
	}

	// float3x3
	if (strcmp(typeName, "float3x3") == 0)
	{
		return 4u * 3 * 3;
	}

	// float4x3
	if (strcmp(typeName, "float4x3") == 0)
	{
		return 4u * 4 * 3;
	}

	// float2x4
	if (strcmp(typeName, "float2x4") == 0)
	{
		return 4u * 2 * 4;
	}

	// float3x4
	if (strcmp(typeName, "float3x4") == 0)
	{
		return 4u * 3 * 4;
	}

	// float4x4
	if (strcmp(typeName, "float4x4") == 0)
	{
		return 4u * 4 * 4;
	}

	// matrix
	if (strcmp(typeName, "matrix") == 0)
	{
		return 4u * 4 * 4;
	}
		
	throw;
}

// source: https://takinginitiative.wordpress.com/2011/12/11/directx-1011-basic-shader-reflection-automatic-input-layout-creation/
HRESULT CreateInputLayoutDescFromVertexShaderSignature(void* shaderData, size_t shaderDataSize, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout)
{
	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = NULL;
	if (FAILED(D3DReflect(shaderData, shaderDataSize, IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
	{
		return S_FALSE;
	}

	// Get shader info
	D3D11_SHADER_DESC shaderDesc;
	pVertexShaderReflection->GetDesc(&shaderDesc);

	// Read input layout description from shader info
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (uint32 i = 0; i< shaderDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

		// fill out input element desc
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		// determine DXGI format
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		//save element desc
		inputLayoutDesc.push_back(elementDesc);
	}

	// try to find cached input layout
	for(auto & il : input_layouts)
	{
		if(il.elements.size() == inputLayoutDesc.size())
		{
			for(auto i = 0u; i < inputLayoutDesc.size(); i ++)
			{
				if(inputLayoutDesc[i].Format == il.elements[i].Format && 
					inputLayoutDesc[i].SemanticIndex == il.elements[i].SemanticIndex &&
					strcmp(inputLayoutDesc[i].SemanticName, il.elements[i].SemanticName) == 0)
				{
					// we have found the same vertex input layout!
					// set the input layout and return
					*pInputLayout = il.inputLayout;
					return S_OK;
				}
			}
		}
	}

	// Try to create Input Layout
	auto hr = pD3DDevice->CreateInputLayout(&inputLayoutDesc[0], static_cast<uint>(inputLayoutDesc.size()), shaderData, shaderDataSize, pInputLayout);

	//Free allocation shader reflection memory
	pVertexShaderReflection->Release();

	// cache the input layout
	auto newIl = InputLayout();
	newIl.elements = inputLayoutDesc;
	newIl.inputLayout = *pInputLayout;
	input_layouts.push_back(newIl);

	return hr;
}

D3D11ShaderProgram* LoadShader(const char* fileName)
{
	// initialize samplers
	if(m_samplerState_pc == nullptr)
	{
		InitializeSamplerStates();
		// TODO: Anisotropic sampler states
	}

	auto device = static_cast<ID3D11Device*>(D3D11Renderer::getDevice());

	File shaderFile = {};
	File::openFile(&shaderFile, fileName, OpenMode::OpenRead);

	shadermeta meta = {};
	meta.read(shaderFile);

	auto passCount = shaderFile.read<int>();

	std::vector<D3D11ShaderPass> shader_passes = {};

	for(auto passId = 0; passId < passCount; passId ++)
	{
		auto shaderPass = D3D11ShaderPass();
		auto pass = meta.passes[passId];
		auto shaderCount = shaderFile.read<int>();

		// copy pass name
		shaderPass.name = pass.name;

		// create shaders
		for (auto shaderId = 0; shaderId < shaderCount; shaderId++)
		{
			auto shaderType = static_cast<SHADER_TYPE>(shaderFile.read<int>());
			
			// read HLSL
			auto hlsl_len = shaderFile.read<int>();
			if (hlsl_len > 1024 * 64) throw;

			auto hlsl_data = new char[hlsl_len];
			shaderFile.read(hlsl_data, hlsl_len);

			// we now have the shader bytecode
			// create shader
			switch(shaderType)
			{
			case PIXEL_SHADER: 
				device->CreatePixelShader(hlsl_data, hlsl_len, nullptr, &shaderPass.m_pixelShader);
				break;
			case VERTEX_SHADER:
			{
				device->CreateVertexShader(hlsl_data, hlsl_len, nullptr, &shaderPass.m_vertexShader);
				CreateInputLayoutDescFromVertexShaderSignature(hlsl_data, static_cast<size_t>(hlsl_len), device, &shaderPass.m_inputLayout);
				break;
			}

			case COMPUTE_SHADER:
				device->CreateComputeShader(hlsl_data, hlsl_len, nullptr, &shaderPass.m_computeShader);
				break;
			default: break;
			}

			delete[] hlsl_data;

			// read GLSL
			auto glsl_len = shaderFile.read<int>();
			if (glsl_len > 1024 * 64) throw;

			auto glsl_data = new char[glsl_len];
			shaderFile.read(glsl_data, glsl_len);

			// we don't need GLSL, but we need to push the stream

			delete[] glsl_data;
		}

		// add shader pass
		shader_passes.push_back(shaderPass);
	}

	// create buffers
	std::vector<D3D11ShaderBuffer> shader_buffers = {};
	for (auto bufferId = 0u; bufferId < meta.buffers.size(); bufferId++)
	{
		auto buffer = meta.buffers[bufferId];

		D3D11ShaderBuffer shaderBuffer = {};

		// copy buffer name
		shaderBuffer.name = buffer.name;

		// copy fields, calculate size and offsets
		uint16_t offset = 0;
		for(auto fieldId = 0u; fieldId < buffer.fields.size(); fieldId++)
		{
			auto field = buffer.fields[fieldId];
			auto fieldSize = TypeSizeFromString(field.type.c_str());

			D3D11ShaderBufferField shaderField = {};

			// copy name
			shaderField.name = field.name;

			// set size
			shaderField.size = fieldSize;

			// set offset
			shaderField.offset = offset;

			// add shader buffer field
			shaderBuffer.fields.push_back(shaderField);

			offset += fieldSize;
		}

		// create dynamic ID3D11Buffer
		// offset is the total size of buffer now
		shaderBuffer.m_size = offset;

		// allocate data for the buffer
		shaderBuffer.m_data = new byte[offset];
		memset(shaderBuffer.m_data, 0, offset);

		// construct buffer description
		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = offset;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA subresource_data = {};
		subresource_data.pSysMem = shaderBuffer.m_data;
		subresource_data.SysMemPitch = 0;
		subresource_data.SysMemSlicePitch = 0;

		// create buffer
		ID3D11Buffer* d3dbuffer = nullptr;
		DXCALL(device->CreateBuffer(&desc, &subresource_data, &d3dbuffer)) throw;

		// set buffer
		shaderBuffer.m_buffer = d3dbuffer;

		// add shader buffer
		shader_buffers.push_back(shaderBuffer);
	}

	return new D3D11ShaderProgram(shader_passes, shader_buffers);
}

void D3D11ShaderProgram::updateBuffers(int buffer_index)
{
	D3D11_MAPPED_SUBRESOURCE m_mappedSubres = {};
	auto deviceContext = static_cast<ID3D11DeviceContext*>(D3D11Renderer::getDeviceContext());

	auto buffer = &m_buffers[buffer_index];

	// map buffer
	DXCALL(deviceContext->Map(buffer->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mappedSubres))
	{
		// Logger::write("Failed to map buffer", LogLevel::Error);
	}

	// upload data
	memcpy(m_mappedSubres.pData, buffer->m_data, buffer->m_size);

	// unmap the buffer
	deviceContext->Unmap(buffer->m_buffer, 0);
}

void D3D11ShaderProgram::Apply(const char* pass_name)
{
	auto idx = 0;
	for (auto & pass : m_passes) 
	{
		if (pass.name == pass_name) 
		{
			Apply(idx);
			return;
		}
		idx++;
	}
}

void D3D11ShaderProgram::Apply(int pass_index)
{
	// select pass
	auto pass = m_passes[pass_index];

	// get deviceContext instance
	auto deviceContext = static_cast<ID3D11DeviceContext*>(D3D11Renderer::getDeviceContext());

	// set buffers
	auto idx = 0;
	for(auto & buffer : m_buffers)
	{
		// check if some value was changed
		if (buffer.m_dirty)
		{
			// this buffer has some changed data,
			// we need to update it now.
			updateBuffers(idx);

			// uncheck dirty flag
			buffer.m_dirty = false;
		}

		idx++;
	}

	// get buffers cache pointer
	auto buffers = m_buffersCache.data();

	// set buffers
	if (pass.m_vertexShader) // for vertex shader
		deviceContext->VSSetConstantBuffers(0u, static_cast<UINT>(m_buffers.size()), buffers);
	if (pass.m_pixelShader) // for pixel shader
		deviceContext->PSSetConstantBuffers(0u, static_cast<UINT>(m_buffers.size()), buffers);
	if (pass.m_computeShader) // for compute shader
		deviceContext->CSSetConstantBuffers(0u, static_cast<UINT>(m_buffers.size()), buffers);

	// set shaders
	if (pass.m_vertexShader) // for vertex shader
		deviceContext->VSSetShader(pass.m_vertexShader, nullptr, 0);
	if (pass.m_pixelShader) // for pixel shader
		deviceContext->PSSetShader(pass.m_pixelShader, nullptr, 0);
	if (pass.m_computeShader) // for compute shader
		deviceContext->CSSetShader(pass.m_computeShader, nullptr, 0);

	// we have only one pointer for every certain input layout
	// for example if we use 10 shaders with the same Input structure
	// there is only one m_inputLayout pointer due to caching.
	if (m_lastInputLayout != pass.m_inputLayout)
	{
		// set input layout
		deviceContext->IASetInputLayout(pass.m_inputLayout);

		// update the layout
		m_lastInputLayout = pass.m_inputLayout;
	}
}

void D3D11ShaderProgram::SetValue(const char* buffer_name, const char* field_name, void* value)
{
	auto idx1 = 0;
	for (auto & buffer : m_buffers) 
	{
		if (buffer.name == buffer_name) 
		{
			auto idx2 = 0;

			for (auto & field : buffer.fields) 
			{
				if (field.name == field_name) 
				{
					SetValue(idx1, idx2, value);
					return;
				}
				idx2++;
			}
			return;
		}
		idx1++;
	}
}

void D3D11ShaderProgram::SetValue(int buffer_index, int field_index, void* value)
{
	// update data
	auto buffer = m_buffers[buffer_index];
	auto field = buffer.fields[field_index];
	
	// copy data
	memcpy(buffer.m_data + field.offset, value, field.size);

	// set dirty flags
	m_buffers[buffer_index].m_dirty = true;
}
