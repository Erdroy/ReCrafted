// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Shader.h"
#include "Common/Logger.h"

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
