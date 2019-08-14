// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Shader.h"
#include "Common/Array.h"
#include "Common/Logger.h"
#include "Rendering/Texture.h"

void Shader::OnInitialize()
{
    m_shaderHandle = Renderer::CreateShader(m_shaderName.StdStr().c_str());
}

void Shader::OnLoadBegin(const std::string& fileName)
{
    m_shaderName = String(fileName.c_str());
}

void Shader::OnDeserializeJson(uint16_t version, const json& json)
{

}

void Shader::OnUnload()
{
    Logger::Log("Unloading shader '{0}'", m_shaderName.StdStr());
    Renderer::DestroyShader(m_shaderHandle);
}

void Shader::SetTextureArray(const int slot, const Array<Texture*>& textureArray)
{
    Renderer::Texture2DHandle handles[32] = {};

    auto i = 0;
    for (auto& texture : textureArray)
        handles[++i] = texture->GetHandle();

    Renderer::ApplyTextureArray2D(handles, slot, textureArray.Count());
}

void Shader::SetTexture(const int slot, Texture* texture)
{
    Renderer::ApplyTexture2D(texture->GetHandle(), slot);
}

void Shader::SetTexture(const int slot, const Renderer::Texture2DHandle texture)
{
    Renderer::ApplyTexture2D(texture, slot);
}
