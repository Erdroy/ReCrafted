// ReCrafted (c) 2016-2018 Always Too Late

#include "PPVignette.h"

void PPVignette::OnInitialize()
{
    m_shader = Shader::LoadShader("../assets/shaders/PPVignette.shader");
}

void PPVignette::Render()
{

}

void PPVignette::Dispose()
{
}

RefPtr<Shader> PPVignette::GetShader() const
{
    return m_shader;
}
