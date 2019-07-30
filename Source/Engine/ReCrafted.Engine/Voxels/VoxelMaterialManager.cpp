// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelMaterialManager.h"
#include "Content/ContentManager.h"
#include "Assets/VoxelMaterial.h"

void VoxelMaterialManager::Initialize()
{
}

void VoxelMaterialManager::Shutdown()
{
}

void VoxelMaterialManager::RegisterMaterial(VoxelMaterial* voxelMaterial)
{
    // Lock
    ScopeLock(m_materialLock);

    ASSERT(voxelMaterial);
    const auto voxelId = voxelMaterial->GetMaterial();
    ASSERT(m_materials.contains(voxelId) == false);

    // Set
    m_materials[voxelId] = voxelMaterial;
}

void VoxelMaterialManager::UnregisterMaterial(VoxelMaterial* voxelMaterial)
{
    // Lock
    ScopeLock(m_materialLock);

    ASSERT(voxelMaterial);
    const auto voxelId = voxelMaterial->GetMaterial();

    // Try to remove from list
    const auto it = m_materials.find(voxelId);
    if (it != m_materials.end())
        m_materials.erase(it);
}

void VoxelMaterialManager::SetDefaultMaterial(VoxelMaterial* material)
{
    GetInstance()->m_defaultMaterial = material;
}

void VoxelMaterialManager::AddMaterial(VoxelMaterial* material)
{
    DEBUG_ASSERT(IsReleased() == false);

    // Lock
    auto& lock = GetInstance()->m_materialLock;
    ScopeLock(lock);

    // Add material
    GetInstance()->m_materials.insert(std::make_pair(material->GetMaterial(), material));
    GetInstance()->m_allMaterials.emplace_back(material);
}

VoxelMaterial* VoxelMaterialManager::GetMaterial(const VoxelMaterial_t id)
{
    DEBUG_ASSERT(IsReleased() == false);

    // Lock
    auto& lock = GetInstance()->m_materialLock;
    ScopeLock(lock);

    if (!GetInstance()->m_materials.contains(id))
    {
        // Return default material
        return GetInstance()->m_defaultMaterial;
    }

    return GetInstance()->m_materials[id];
}
