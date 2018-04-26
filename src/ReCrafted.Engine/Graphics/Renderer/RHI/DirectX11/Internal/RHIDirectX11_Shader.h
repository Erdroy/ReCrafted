// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef RHIDIRECTX11_SHADER_H
#define RHIDIRECTX11_SHADER_H

#include "../DirectX11.h"
#include "../../../RendererConfig.h"

#include <json.hpp>
#include "Scripting/Assembly.h"

namespace Renderer
{
    class RHIDirectX11_Shader
    {
        friend class RHIDirectX11;

    private:
        class Buffer
        {
        public:
            class Field
            {
            public:
                std::string m_name = {};
                uint16_t m_size = 0u;
                uint16_t m_offset = 0u;
            };

        public:
            std::string m_name = {};
            std::vector<Field> m_fields = {};
            byte* m_data = nullptr;
            uint16_t m_size = 0u;
            uint8_t m_index = 0u;

            ID3D11Buffer* m_buffer = nullptr;

        public:
            ~Buffer()
            {
                delete[] m_data;
                SafeRelease(m_buffer);
            }
        };

        class Pass
        {
        public:
            std::string m_name = {};

            ID3D11InputLayout* m_inputLayout = nullptr;

            std::string m_vsName = {};
            ID3D11VertexShader* m_vertexShader = nullptr;
            std::vector<Buffer> m_vsBuffers = {};

            std::string m_psName = {};
            ID3D11PixelShader* m_pixelShader = nullptr;
            std::vector<Buffer> m_psBuffers = {};

            std::string m_csName = {};
            ID3D11ComputeShader* m_computeShader = nullptr;
            std::vector<Buffer> m_csBuffers = {};

        public:
            ~Pass()
            {
                SafeRelease(m_vertexShader);
                SafeRelease(m_pixelShader);
                SafeRelease(m_computeShader);
            }
        };

    private:
        std::string m_name = {};
        std::string m_desc = {};

        std::vector<Buffer> m_buffers = {};
        std::vector<Pass> m_passes = {};

        uint m_stride = 0u;

    private:
        void BindPass(ID3D11DeviceContext* context, Pass& pass);

    public:
        void Bind(ID3D11DeviceContext* context, int passId);
        void Bind(ID3D11DeviceContext* context, std::string passName);

        void BindTexture(ID3D11DeviceContext* context, uint8_t slot, ID3D11Texture2D* texture);
        void BindTexture(ID3D11DeviceContext* context, uint8_t slot, ID3D11Texture3D* texture);
        void BindTexture(ID3D11DeviceContext* context, std::string name, ID3D11Texture2D* texture);
        void BindTexture(ID3D11DeviceContext* context, std::string name, ID3D11Texture3D* texture);

        void BindBuffer(ID3D11DeviceContext* context, uint8_t slot, ID3D11Buffer* buffer);
        void BindBuffer(ID3D11DeviceContext* context, std::string name, ID3D11Buffer* buffer);

        void BindBlendState(ID3D11DeviceContext* context, uint8_t slot, ID3D11BlendState* state);
        void BindBlendState(ID3D11DeviceContext* context, std::string name, ID3D11BlendState* state);

        void ApplyChanges();

        uint GetStride();

        void Release();

    public:
        template<typename T>
        void SetValue(int buffer, int index, T value)
        {

        }

    public:
        static RHIDirectX11_Shader* Create(ID3D11Device* device, nlohmann::json& shaderJson);
    };
}

#endif // RHIDIRECTX11_SHADER_H
