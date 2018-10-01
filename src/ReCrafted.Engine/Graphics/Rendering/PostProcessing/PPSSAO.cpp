// ReCrafted (c) 2016-2018 Always Too Late

#include "PPSSAO.h"
#include "Content/ContentManager.h"

void PPSSAO::OnInitialize()
{
    m_shader = ContentManager::LoadAsset<Shader>("Shaders/PPSSAO");
}

void PPSSAO::Render()
{

}

void PPSSAO::Dispose()
{
}

Shader* PPSSAO::GetShader() const
{
    return m_shader;
}
