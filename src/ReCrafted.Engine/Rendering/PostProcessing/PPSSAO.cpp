// ReCrafted (c) 2016-2018 Always Too Late

#include "PPSSAO.h"

void PPSSAO::OnInitialize()
{
    m_shader = Shader::LoadShader("../assets/shaders/PPSSAO.shader");
}

void PPSSAO::Render()
{

}

void PPSSAO::Dispose()
{
}

RefPtr<Shader> PPSSAO::GetShader() const
{
    return m_shader;
}
