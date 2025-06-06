// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include "../DirectX11.h"
#include "../../../RendererConfig.h"

#include <nlohmann/json.hpp>

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

            bool m_dirty = false;

            ID3D11Buffer* m_buffer = nullptr;

        public:
            ~Buffer()
            {
                if (m_data)
                {
                    delete[] m_data;
                    m_data = nullptr;
                }
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
            std::vector<Buffer*> m_vsBuffers = {};

            std::string m_psName = {};
            ID3D11PixelShader* m_pixelShader = nullptr;
            std::vector<Buffer*> m_psBuffers = {};

            std::string m_csName = {};
            ID3D11ComputeShader* m_computeShader = nullptr;
            std::vector<Buffer*> m_csBuffers = {};

            std::string m_gsName = {};
            ID3D11GeometryShader* m_geometryShader = nullptr;
            std::vector<Buffer*> m_gsBuffers = {};
        public:
            ~Pass()
            {
                SafeRelease(m_vertexShader);
                SafeRelease(m_pixelShader);
                SafeRelease(m_computeShader);
                SafeRelease(m_geometryShader);
            }
        };

    private:
        std::string m_name = {};
        std::string m_desc = {};

        std::vector<RefPtr<Buffer>> m_buffers = {};
        std::vector<RefPtr<Pass>> m_passes = {};

        //std::vector<std::string> m_textures2d = {};
        std::vector<ID3D11SamplerState*> m_samplers = {};

        bool m_dirty = false;

        uint m_stride = 0u;

    private:
        void BindPass(ID3D11DeviceContext* context, const Pass* pass);

    public:
        void Bind(ID3D11DeviceContext* context, int passId);
        void Bind(ID3D11DeviceContext* context, std::string passName);

        void BindResource(ID3D11DeviceContext* context, uint8_t slot, ID3D11ShaderResourceView* srv);
        void BindResources(ID3D11DeviceContext* context, uint8_t slot, ID3D11ShaderResourceView** srvs, uint8_t srvCount);
        //void BindResource(ID3D11DeviceContext* context, std::string name, ID3D11ShaderResourceView* srv);

        //void BindBuffer(ID3D11DeviceContext* context, uint8_t slot, ID3D11Buffer* buffer);
        //void BindBuffer(ID3D11DeviceContext* context, std::string name, ID3D11Buffer* buffer);

        //void BindBlendState(ID3D11DeviceContext* context, uint8_t slot, ID3D11BlendState* state);
        //void BindBlendState(ID3D11DeviceContext* context, std::string name, ID3D11BlendState* state);

        void SetValue(int buffer, int index, void* data, size_t dataSize);
        void SetValues(int buffer, void* data, size_t dataSize, size_t dataOffset);

        void ApplyChanges(ID3D11DeviceContext* context);

        uint GetStride();

        void Release();

    public:
        template <typename T>
        void SetValue(int buffer, int index, T value)
        {
            SetValue(buffer, index, &value, sizeof T);
        }

    public:
        static RHIDirectX11_Shader* Create(ID3D11Device* device, nlohmann::json& shaderJson);
    };
}
