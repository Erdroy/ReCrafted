// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "D3D11ShaderProgram.h"

#include "../../../../ReCrafted.ShaderCompiler/Compiler/ShaderMeta.h"
#include "../../../../ReCrafted.ShaderCompiler/Compiler/File.h"

#include "D3D11Renderer.h"

struct InputLayout
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
	ID3D11InputLayout* inputLayout;
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

	// TODO: try to find the same input layout, and return it's pointer when found, in any other case, continue.

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

void D3D11ShaderProgram::Apply(const char* pass_name)
{
	for (auto & pass : m_passes) {
		if (pass.name == pass_name) {
			auto deviceContext = static_cast<ID3D11DeviceContext*>(D3D11Renderer::getDeviceContext());

			if(pass.m_computeShader)
				deviceContext->CSSetShader(pass.m_computeShader, nullptr, 0);
			if (pass.m_pixelShader)
				deviceContext->PSSetShader(pass.m_pixelShader, nullptr, 0);
			if (pass.m_vertexShader)
				deviceContext->VSSetShader(pass.m_vertexShader, nullptr, 0);

			deviceContext->IASetInputLayout(pass.m_inputLayout);
			return;
		}
	}
}

D3D11ShaderProgram* LoadShader(const char* fileName)
{
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

		shader_passes.push_back(shaderPass);
	}

	// create buffers
	// TODO: create buffers

	return new D3D11ShaderProgram(shader_passes);
}
