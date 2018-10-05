// ReCrafted (c) 2016-2018 Always Too Late

#include "Shader.h"
#include "Core/Logger.h"

#include <json.hpp>

void Shader::OnInitialize()
{
    m_shaderHandle = Renderer::CreateShader(m_shaderName.StdStr().c_str());
}

void Shader::OnLoadBegin(const std::string& fileName)
{
    m_shaderName = Text(fileName.c_str());
}

void Shader::OnDeserializeJson(uint16_t version, const json& json)
{

}

void Shader::OnUnload()
{
    Logger::LogInfo("Unloading shader '{0}'", m_shaderName.StdStr());
    Renderer::DestroyShader(m_shaderHandle);
}
