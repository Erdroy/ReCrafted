// ReCrafted (c) 2016-2018 Always Too Late

#include "Shader.h"
#include "../Platform/Platform.h"

#include "Core/Logger.h"

#include <json.hpp>

void Shader::dispose()
{
	Logger::logInfo("Unloading shader '{0}'", m_shaderName);
    Renderer::DestroyShader(m_shaderHandle);
}

Ref<Shader> Shader::loadShader(const char* shaderName)
{
	Ref<Shader> shader(new Shader);

	Logger::logInfo("Loading shader {0}", shaderName);
	strcpy_s(shader->m_shaderName, shaderName);
    shader->m_shaderHandle = Renderer::CreateShader(shaderName);
	return shader;
}
