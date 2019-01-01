// ReCrafted (c) 2016-2019 Always Too Late

#include "PPVignette.h"
#include "Content/ContentManager.h"

void PPVignette::OnInitialize()
{
    m_shader = ContentManager::LoadAsset<Shader>("Shaders/PPVignette");
}

void PPVignette::Render()
{

}

void PPVignette::Dispose()
{
}

Shader* PPVignette::GetShader() const
{
    return m_shader;
}
