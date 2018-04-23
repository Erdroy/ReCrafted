// GFXL - Graphics Library (c) 2016-2017 Damian 'Erdroy' Korczowski

#include "RHIDirectX11_Shader.h"
#include "../../../../GFXL.hpp"
#include "Core/Lock.h"
#include <base64.h>

namespace GFXL
{
    struct InputLayout
    {
        std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
        CComPtr<ID3D11InputLayout> inputLayout;
    };

    Lock m_inputLayoutLock = {};
    std::vector<InputLayout> m_inputLayouts = {};
    void* m_lastInputLayout = nullptr;

    uint D3D11HLSLFixBufferSize(uint size)
    {
        return (size + 64 - 1) / 64 * 64;
    }

    // source: https://takinginitiative.wordpress.com/2011/12/11/directx-1011-basic-shader-reflection-automatic-input-layout-creation/
    HRESULT D3D11CreateInputLayout(ID3D11Device* pD3DDevice, void* shaderData, size_t shaderDataSize, ID3D11InputLayout** pInputLayout)
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
        for (uint32_t i = 0; i< shaderDesc.InputParameters; i++)
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
        ScopeLock(m_inputLayoutLock);
        for (var & il : m_inputLayouts)
        {
            if (il.elements.size() == inputLayoutDesc.size())
            {
                for (var i = 0u; i < inputLayoutDesc.size(); i++)
                {
                    if (inputLayoutDesc[i].Format == il.elements[i].Format &&
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
        var hr = pD3DDevice->CreateInputLayout(&inputLayoutDesc[0], static_cast<UINT>(inputLayoutDesc.size()), shaderData, shaderDataSize, pInputLayout);

        _ASSERT(SUCCEEDED(hr));

        //Free allocation shader reflection memory
        pVertexShaderReflection->Release();

        // cache the input layout
        var newIl = InputLayout();
        newIl.elements = inputLayoutDesc;
        newIl.inputLayout = *pInputLayout;
        m_inputLayouts.push_back(newIl);

        return hr;
    }

    void RHIDirectX11_Shader::BindPass(ID3D11DeviceContext* context, Pass& pass)
    {
        if (pass.m_vertexShader)
            context->VSSetShader(pass.m_vertexShader, nullptr, 0);

        if (pass.m_pixelShader)
            context->PSSetShader(pass.m_pixelShader, nullptr, 0);

        if (pass.m_computeShader)
            context->CSSetShader(pass.m_computeShader, nullptr, 0);

        // TODO: Set constant buffers
        // TODO: Set sampler states

        // we have only one pointer for every certain input layout
        // for example if we use 10 shaders with the same Input structure
        // there is only one m_inputLayout pointer due to caching.
        if (m_lastInputLayout != pass.m_inputLayout)
        {
            // set input layout
            context->IASetInputLayout(pass.m_inputLayout);

            // update the layout
            m_lastInputLayout = pass.m_inputLayout;
        }
    }

    void RHIDirectX11_Shader::Bind(ID3D11DeviceContext* contenxt, int passId)
    {
        // TODO: Handle invalid passId

        var& pass = m_passes[passId];
        BindPass(contenxt, pass);
    }

    void RHIDirectX11_Shader::Bind(ID3D11DeviceContext* contenxt, std::string passName)
    {
        for(var i = 0u; i < m_passes.size(); i ++)
        {
            var& pass = m_passes[i];
            if(pass.m_name == passName)
            {
                BindPass(contenxt, pass);
                return;
            }
        }
    }

    void RHIDirectX11_Shader::Release()
    {
        m_buffers.clear();
        m_passes.clear();
        delete this;
    }

    RHIDirectX11_Shader* RHIDirectX11_Shader::Create(ID3D11Device* device, nlohmann::json& shaderJson)
    {
        var shader = new RHIDirectX11_Shader();
        var buffers = shaderJson["UniformBuffers"];
        var passes = shaderJson["Passes"];

        shader->m_name = shaderJson["Name"].get<std::string>();
        shader->m_desc = shaderJson["Description"].get<std::string>();

        // Create all passes
        for (rvar passJson : passes)
        {
            // create pass instance and get it's ref
            shader->m_passes.push_back({});
            rvar pass = shader->m_passes[shader->m_passes.size() - 1];

            // set pass name
            pass.m_name = passJson["Name"].get<std::string>();
            
            // try to create vertex shader
            var vsBC = passJson["VSByteCode"];
            if (vsBC.is_string())
            {
                var byteCode = base64_decode(vsBC.get<std::string>());
                var hr = device->CreateVertexShader(byteCode.data(), byteCode.size(), nullptr, &pass.m_vertexShader);

                _ASSERT(SUCCEEDED(hr));

                // every vertex shader needs input layout to be bound to the GPU, 
                // so create or use cached input layout
                hr = D3D11CreateInputLayout(device, byteCode.data(), byteCode.size(), &pass.m_inputLayout);

                _ASSERT(SUCCEEDED(hr));
            }

            // try to create pixel shader
            var psBC = passJson["PSByteCode"];
            if (psBC.is_string())
            {
                var byteCode = base64_decode(psBC.get<std::string>());
                var hr = device->CreatePixelShader(byteCode.data(), byteCode.size(), nullptr, &pass.m_pixelShader);

                _ASSERT(SUCCEEDED(hr));
            }

            // try to create compute shader
            var csBC = passJson["CSByteCode"];
            if(csBC.is_string())
            {
                var byteCode = base64_decode(csBC.get<std::string>());
                var hr = device->CreateComputeShader(byteCode.data(), byteCode.size(), nullptr, &pass.m_computeShader);

                _ASSERT(SUCCEEDED(hr));
            }
        }

        // Create constant buffers
        for (rvar bufferJson : buffers)
        {
            // create buffer instance and get it's ref
            shader->m_buffers.push_back({});
            rvar buffer = shader->m_buffers[shader->m_buffers.size() - 1];

            // set buffer name
            buffer.m_name = bufferJson["Name"].get<std::string>();
            
            var fieldsArray = bufferJson["Uniforms"];

            var fieldOffset = 0u;
            for (rvar fieldData : fieldsArray)
            {
                buffer.m_fields.push_back({});
                rvar field = buffer.m_fields[buffer.m_fields.size() - 1];

                cvar name = fieldData["Name"].get<std::string>();
                cvar size = fieldData["Size"].get<int>();

                // set buffer name
                field.m_name = name;

                // set buffer size
                field.m_size = size;

                // set field offset
                field.m_offset = fieldOffset;

                // add field offset
                fieldOffset += static_cast<uint>(size);
            }

            // field offset is not the size of the buffer
            // but we still need to round int up to multiply of 64
            cvar bufferSize = D3D11HLSLFixBufferSize(fieldOffset);
            buffer.m_size = bufferSize;

            // create buffer data
            buffer.m_data = new byte[bufferSize];

            // create D3D11 buffer
            D3D11_BUFFER_DESC desc = {};
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.ByteWidth = bufferSize;
            desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

            D3D11_SUBRESOURCE_DATA subresource_data = {};
            subresource_data.pSysMem = buffer.m_data;
            subresource_data.SysMemPitch = 0;
            subresource_data.SysMemSlicePitch = 0;

            var hr = device->CreateBuffer(&desc, &subresource_data, &buffer.m_buffer);

            _ASSERT(SUCCEEDED(hr));

            // TODO: select targets
        }
        
        // TODO: Setup sampler states

        return shader;
    }
}
