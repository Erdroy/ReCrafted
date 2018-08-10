// ReCrafted (c) 2016-2018 Always Too Late

#include "RHIDirectX11_Shader.h"
#include "../../../Renderer.hpp"

#include <base64.h>

namespace Renderer
{
    struct InputLayout
    {
        std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
        CComPtr<ID3D11InputLayout> inputLayout;
    };

    struct SamplerState
    {
        std::string name;
        ID3D11SamplerState* sampler;
    };

    // == stats ==
    namespace RHI
    {
        extern uint32_t m_apiCalls;
    }

    std::vector<InputLayout> m_inputLayouts = {};
    std::vector<SamplerState> m_samplerStates = {};

    // source: https://takinginitiative.wordpress.com/2011/12/11/directx-1011-basic-shader-reflection-automatic-input-layout-creation/
    HRESULT D3D11CreateInputLayout(ID3D11Device* pD3DDevice, void* shaderData, size_t shaderDataSize, uint* stride,
                                   ID3D11InputLayout** pInputLayout)
    {
        // Reflect shader info
        ID3D11ShaderReflection* pVertexShaderReflection = NULL;
        if (FAILED(D3DReflect(shaderData, shaderDataSize, IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection))
        )
        {
            return S_FALSE;
        }

        // Get shader info
        D3D11_SHADER_DESC shaderDesc;
        pVertexShaderReflection->GetDesc(&shaderDesc);

        // Read input layout description from shader info
        std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
        for (uint32_t i = 0; i < shaderDesc.InputParameters; i++)
        {
            D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
            DX_CALL(pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc));

            // Skip all SV_*
            if (paramDesc.SystemValueType != D3D_NAME_UNDEFINED)
                continue;

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
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format =
                    DXGI_FORMAT_R32_SINT;
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format =
                    DXGI_FORMAT_R32_FLOAT;
            }
            else if (paramDesc.Mask <= 3)
            {
                if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format =
                    DXGI_FORMAT_R32G32_UINT;
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format =
                    DXGI_FORMAT_R32G32_SINT;
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format =
                    DXGI_FORMAT_R32G32_FLOAT;
            }
            else if (paramDesc.Mask <= 7)
            {
                if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format =
                    DXGI_FORMAT_R32G32B32_UINT;
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format =
                    DXGI_FORMAT_R32G32B32_SINT;
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format =
                    DXGI_FORMAT_R32G32B32_FLOAT;
            }
            else if (paramDesc.Mask <= 15)
            {
                if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format =
                    DXGI_FORMAT_R32G32B32A32_UINT;
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format =
                    DXGI_FORMAT_R32G32B32A32_SINT;
                else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format =
                    DXGI_FORMAT_R32G32B32A32_FLOAT;
            }

            //save element desc
            inputLayoutDesc.push_back(elementDesc);
            *stride += DXGIFormatGetSize(elementDesc.Format);
        }

        // try to find cached input layout
        for (rvar il : m_inputLayouts)
        {
            if (il.elements.size() == inputLayoutDesc.size())
            {
                for (var i = 0u; i < inputLayoutDesc.size(); i++)
                {
                    rvar element = il.elements[i];
                    rvar current = inputLayoutDesc[i];

                    if (current.Format != element.Format)
                        break;

                    if (current.SemanticIndex != element.SemanticIndex)
                        break;

                    // we have found the same vertex input layout!
                    // set the input layout and return
                    *pInputLayout = il.inputLayout;
                    return S_OK;
                }
            }
        }

        // Try to create Input Layout
        DX_CALL(pD3DDevice->CreateInputLayout(&inputLayoutDesc[0], static_cast<UINT>(inputLayoutDesc.size()),
                                               shaderData, shaderDataSize, pInputLayout));

        // Free allocation shader reflection memory
        pVertexShaderReflection->Release();

        // cache the input layout
        var newIl = InputLayout();
        newIl.elements = inputLayoutDesc;
        newIl.inputLayout = *pInputLayout;
        m_inputLayouts.push_back(newIl);

        return S_OK;
    }

    HRESULT D3D11CreateSamplerState(ID3D11Device* pD3DDevice, std::string& samplerType,
                                    ID3D11SamplerState** samplerState)
    {
        // check if we already have this sampler type created
        for (rvar sampler : m_samplerStates)
        {
            if (sampler.name == samplerType)
            {
                *samplerState = sampler.sampler;
                return S_OK;
            }
        }

        /*
         * PointClamped, PointMirror, PointWrap
         * LinearClamped, LinearMirror, LinearWrap
         * TODO: AnisotropicClamped, AnisotropicMirror, AnisotropicWrap
         */

        CD3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.MipLODBias = 0.0f;
        samplerDesc.MaxAnisotropy = 16;
        samplerDesc.BorderColor[0] = 0;
        samplerDesc.BorderColor[1] = 0;
        samplerDesc.BorderColor[2] = 0;
        samplerDesc.BorderColor[3] = 0;
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;

        if (samplerType == "PointClamped")
        {
            samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
            samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
            samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
            samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        }
        else if (samplerType == "PointWrap")
        {
            samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
            samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        }
        else if (samplerType == "PointMirror")
        {
            samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
            samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
            samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
            samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
        }
        else if (samplerType == "LinearClamped")
        {
            samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
            samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
            samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        }
        else if (samplerType == "LinearWrap")
        {
            samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        }
        else if (samplerType == "LinearMirror")
        {
            samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
            samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
            samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
        }

        // create sampler
        ID3D11SamplerState* sampler;
        DX_CALL(pD3DDevice->CreateSamplerState(&samplerDesc, &sampler));

        // add created sampler
        m_samplerStates.push_back({samplerType, sampler});

        // return sampler state
        *samplerState = sampler;
        return S_OK;
    }

    void RHIDirectX11_Shader::BindPass(ID3D11DeviceContext* context, Pass& pass)
    {
        // for constant buffers we don't need to check index value as there is only one index per buffer, 
        // so there is no way that someone will get overlapping indices.

        // set vertex shader
        if (pass.m_vertexShader)
        {
            context->VSSetShader(pass.m_vertexShader, nullptr, 0);
            ADD_APICALL();

            // set constant buffers
            if (!pass.m_vsBuffers.empty())
            {
                for (rvar buffer : pass.m_vsBuffers) // TODO: apply all buffers in single API call
                {
                    ID3D11Buffer* buffers[] = {buffer->m_buffer};
                    context->VSSetConstantBuffers(buffer->m_index, 1u, buffers);
                    ADD_APICALL();
                }
            }
            else
            {
                // bind all buffers instead
                for (rvar buffer : m_buffers) // TODO: apply all buffers in single API call
                {
                    ID3D11Buffer* buffers[] = {buffer.m_buffer};
                    context->VSSetConstantBuffers(buffer.m_index, 1u, buffers);
                    ADD_APICALL();
                }
            }
        }
        else // Reset if none
            context->VSSetShader(nullptr, nullptr, 0);

        // set pixel shader
        if (pass.m_pixelShader)
        {
            context->PSSetShader(pass.m_pixelShader, nullptr, 0);
            ADD_APICALL();

            // set constant buffers
            if (!pass.m_psBuffers.empty())
            {
                for (rvar buffer : pass.m_psBuffers)
                {
                    ID3D11Buffer* buffers[] = {buffer->m_buffer};
                    context->PSSetConstantBuffers(buffer->m_index, 1u, buffers);
                    ADD_APICALL();
                }
            }
            else
            {
                // bind all buffers instead
                for (rvar buffer : m_buffers) // TODO: apply all buffers in single API call
                {
                    ID3D11Buffer* buffers[] = {buffer.m_buffer};
                    context->PSSetConstantBuffers(buffer.m_index, 1u, buffers);
                    ADD_APICALL();
                }
            }
        }
        else // Reset if none
            context->PSSetShader(nullptr, nullptr, 0);

        // set compute shader
        if (pass.m_computeShader)
        {
            context->CSSetShader(pass.m_computeShader, nullptr, 0);
            ADD_APICALL();

            // set constant buffers 
            if (!pass.m_csBuffers.empty())
            {
                for (rvar buffer : pass.m_csBuffers)
                {
                    ID3D11Buffer* buffers[] = {buffer->m_buffer};
                    context->CSSetConstantBuffers(buffer->m_index, 1u, buffers);
                    ADD_APICALL();
                }
            }
            else
            {
                // bind all buffers instead
                for (rvar buffer : m_buffers) // TODO: apply all buffers in single API call
                {
                    ID3D11Buffer* buffers[] = {buffer.m_buffer};
                    context->CSSetConstantBuffers(buffer.m_index, 1u, buffers);
                    ADD_APICALL();
                }
            }
        }
        else // Reset if none
            context->CSSetShader(nullptr, nullptr, 0);

        // set geometry shader
        if (pass.m_geometryShader)
        {
            context->GSSetShader(pass.m_geometryShader, nullptr, 0);
            ADD_APICALL();

            // set constant buffers 
            if (!pass.m_gsBuffers.empty())
            {
                for (rvar buffer : pass.m_gsBuffers)
                {
                    ID3D11Buffer* buffers[] = { buffer->m_buffer };
                    context->GSSetConstantBuffers(buffer->m_index, 1u, buffers);
                    ADD_APICALL();
                }
            }
            else
            {
                // bind all buffers instead
                for (rvar buffer : m_buffers) // TODO: apply all buffers in single API call
                {
                    ID3D11Buffer* buffers[] = { buffer.m_buffer };
                    context->GSSetConstantBuffers(buffer.m_index, 1u, buffers);
                    ADD_APICALL();
                }
            }
        }
        else // Reset if none
            context->GSSetShader(nullptr, nullptr, 0);

        // apply shader changes
        if (m_dirty)
            ApplyChanges(context);

        // apply sampler states (PixelShader only)
        context->PSSetSamplers(0u, static_cast<uint>(m_samplers.size()), m_samplers.data());
        ADD_APICALL();

        // set input layout
        context->IASetInputLayout(pass.m_inputLayout);
        ADD_APICALL();
    }

    void RHIDirectX11_Shader::Bind(ID3D11DeviceContext* context, int passId)
    {
        ASSERT(passId < m_passes.size());

        var& pass = m_passes[passId];
        BindPass(context, pass);
    }

    void RHIDirectX11_Shader::Bind(ID3D11DeviceContext* context, std::string passName)
    {
        for (var i = 0u; i < m_passes.size(); i ++)
        {
            var& pass = m_passes[i];
            if (pass.m_name == passName)
            {
                BindPass(context, pass);
                return;
            }
        }
    }

    void RHIDirectX11_Shader::BindResource(ID3D11DeviceContext* context, uint8_t slot, ID3D11ShaderResourceView* srv)
    {
        // apply shader resource (PixelShader only)
        ID3D11ShaderResourceView* srvs[] = {srv};
        context->PSSetShaderResources(slot, 1, srvs);
        ADD_APICALL();
    }

    void RHIDirectX11_Shader::SetValue(int buffer, int index, void* data, size_t dataSize)
    {
        ASSERT(buffer < m_buffers.size());

        // Get buffer description
        rvar bufferDesc = m_buffers[buffer];
        ASSERT(bufferDesc.m_size >= dataSize);
        ASSERT(index < bufferDesc.m_fields.size());

        // Get field description
        rvar fieldDesc = bufferDesc.m_fields[index];
        ASSERT(fieldDesc.m_size == dataSize);

        // Copy new data
        memcpy(bufferDesc.m_data + fieldDesc.m_offset, data, dataSize);

        // Set dirty flag
        bufferDesc.m_dirty = true;

        m_dirty = true;
    }

    void RHIDirectX11_Shader::ApplyChanges(ID3D11DeviceContext* context)
    {
        for (rvar buffer : m_buffers)
        {
            if (buffer.m_dirty)
            {
                D3D11_MAPPED_SUBRESOURCE m_mappedSubres;

                // map buffer
                DX_CALL(context->Map(buffer.m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mappedSubres));

                // upload buffer data
                memcpy(m_mappedSubres.pData, buffer.m_data, buffer.m_size);

                // unmap buffer
                context->Unmap(buffer.m_buffer, 0);

                buffer.m_dirty = false;
            }
        }

        m_dirty = false;
    }

    uint RHIDirectX11_Shader::GetStride()
    {
        return m_stride;
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

        if (!shaderJson["Name"].is_null())
            shader->m_name = shaderJson["Name"].get<std::string>();

        if (!shaderJson["Description"].is_null())
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
                DX_CALL(device->CreateVertexShader(byteCode.data(), byteCode.size(), nullptr, &pass.m_vertexShader));

                // every vertex shader needs input layout to be bound to the GPU, 
                // so create or use cached input layout
                DX_CALL(D3D11CreateInputLayout(device, byteCode.data(), byteCode.size(), &shader->m_stride,
                                            &pass.m_inputLayout));

                // set function name
                pass.m_vsName = passJson["VSFunction"].get<std::string>();
            }

            // try to create pixel shader
            var psBC = passJson["PSByteCode"];
            if (psBC.is_string())
            {
                var byteCode = base64_decode(psBC.get<std::string>());
                DX_CALL(device->CreatePixelShader(byteCode.data(), byteCode.size(), nullptr, &pass.m_pixelShader));

                // set function name
                pass.m_psName = passJson["PSFunction"].get<std::string>();
            }

            // try to create compute shader
            var csBC = passJson["CSByteCode"];
            if (csBC.is_string())
            {
                var byteCode = base64_decode(csBC.get<std::string>());
                DX_CALL(device->CreateComputeShader(byteCode.data(), byteCode.size(), nullptr, &pass.m_computeShader));

                // set function name
                pass.m_csName = passJson["CSFunction"].get<std::string>();
            }

            // try to create geometry shader
            var gsBC = passJson["GSByteCode"];
            if (gsBC.is_string())
            {
                var byteCode = base64_decode(gsBC.get<std::string>());
                DX_CALL(device->CreateGeometryShader(byteCode.data(), byteCode.size(), nullptr, &pass.m_geometryShader));

                // set function name
                pass.m_gsName = passJson["GSFunction"].get<std::string>();
            }
        }

        // Create constant buffers
        for (rvar bufferJson : buffers)
        {
            // create buffer instance and get it's ref
            shader->m_buffers.push_back({});

            cvar bufferIndex = static_cast<uint>(shader->m_buffers.size()) - 1;
            cvar bufferName = bufferJson["Name"].get<std::string>();
            cvar bufferTargets = bufferJson["Targets"];
            cvar bufferUniforms = bufferJson["Uniforms"];
            rvar buffer = shader->m_buffers[bufferIndex];

            // set buffer index
            buffer.m_index = bufferIndex;

            // set buffer name
            buffer.m_name = bufferName;

            for (rvar fieldData : bufferUniforms)
            {
                buffer.m_fields.push_back({});
                rvar field = buffer.m_fields[buffer.m_fields.size() - 1];

                cvar name = fieldData["Name"].get<std::string>();
                cvar size = fieldData["Size"].get<int>();

                // set buffer name
                field.m_name = name;

                // set buffer size
                field.m_size = size;
            }

            // Align fields
            var padLeft = 0u;
            var fieldOffset = 0u;
            for(rvar field : buffer.m_fields)
            {
                // Check if we need to match aligment rules
                if (padLeft < field.m_size)
                {
                    // We need to pad to match the 16-byte aligment
                    fieldOffset += padLeft;

                    if (field.m_size % 16 != 0)
                        padLeft = 16 - (field.m_size % 16);
                    else
                        padLeft = 0u;
                }
                else
                {
                    // Remove field size from pad offset
                    padLeft -= field.m_size;
                }

                // Set field's offset
                field.m_offset = fieldOffset;

                // Add the field size to the field offset
                fieldOffset += field.m_size;
            }

            // field offset is not the size of the buffer
            // but we still need to round int up to multiply of 64
            cvar bufferSize = HLSLFixBufferSize(fieldOffset);
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

            DX_CALL(device->CreateBuffer(&desc, &subresource_data, &buffer.m_buffer));

            // select targets and add to all passes shader functions
            for (rvar pass : shader->m_passes)
            {
                for (rvar funcName : bufferTargets)
                {
                    cvar name = funcName.get<std::string>();

                    // add VS function target
                    if (name == pass.m_vsName)
                        pass.m_vsBuffers.push_back(&buffer);

                    // add PS function target
                    if (name == pass.m_psName)
                        pass.m_psBuffers.push_back(&buffer);

                    // add CS function target
                    if (name == pass.m_csName)
                        pass.m_csBuffers.push_back(&buffer);

                    // add GS function target
                    if (name == pass.m_gsName)
                        pass.m_gsBuffers.push_back(&buffer);
                }
            }
        }

        // TODO: Load uniforms (textures, buffers, samplers etc.) - needed to allow binding by name
        //var textures2d = shaderJson["Textures2D"];
        //for (rvar texture : textures2d)
        //    shader->m_textures2d.push_back(texture.get<std::string>());

        var samplers = shaderJson["SamplerStates"];
        for (rvar sampler : samplers)
        {
            var samplerName = sampler.get<std::string>();

            ID3D11SamplerState* samplerState;
            DX_CALL(D3D11CreateSamplerState(device, samplerName, &samplerState));

            shader->m_samplers.push_back(samplerState);
        }

        return shader;
    }
}
