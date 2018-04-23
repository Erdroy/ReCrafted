// GFXL - Graphics Library (c) 2016-2017 Damian 'Erdroy' Korczowski

#pragma once

#ifndef RHIDIRECTX11_SHADER_H
#define RHIDIRECTX11_SHADER_H

#include "../DirectX11.h"
#include "../../../../GFXLConfig.h"

#include <json.hpp>

namespace GFXL
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

        public:
            ~Buffer()
            {
                delete[] m_data;
            }
        };

        class Pass
        {
        public:
            std::string m_name = {};

            ID3D11InputLayout* m_inputLayout = nullptr;

            std::string m_vsName = {};
            ID3D11VertexShader* m_vertexShader = nullptr;

            std::string m_psName = {};
            ID3D11PixelShader* m_pixelShader = nullptr;

            std::string m_csName = {};
            ID3D11ComputeShader* m_computeShader = nullptr;

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

    private:
        void BindPass(ID3D11DeviceContext* context, Pass& pass);

    public:
        void Bind(ID3D11DeviceContext* context, int passId);
        void Bind(ID3D11DeviceContext* context, std::string passName);
        void Release();

    public:
        static RHIDirectX11_Shader* Create(ID3D11Device* device, nlohmann::json& shaderJson);
    };
}

#endif // RHIDIRECTX11_SHADER_H
