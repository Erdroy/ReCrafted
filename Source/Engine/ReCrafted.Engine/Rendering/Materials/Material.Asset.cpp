// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Material.h"

void Material::OnDeserializeJson(uint16_t version, const json& json)
{
    // TODO: Load textures
    // TODO: Load and allocate fields
}

void Material::OnUnload()
{
    // Destroy textures
    for(auto&& texture : m_textures)
        Destroy(texture);
    m_textures.Clear();

    // TODO: Make sure that m_fields will not make any memory leaks
}
