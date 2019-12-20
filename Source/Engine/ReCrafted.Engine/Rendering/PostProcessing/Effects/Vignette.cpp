// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Vignette.h"
#include "Content/ContentManager.h"
#include "Rendering/Shader.h"

Vignette::~Vignette()
{
    ContentManager::UnloadAssetSafe(m_shader);
}

void Vignette::OnInitialize()
{
    m_shader = ContentManager::LoadAsset<Shader>("Shaders/PPVignette");
}

Shader* Vignette::GetShader() const
{
    return m_shader;
}
