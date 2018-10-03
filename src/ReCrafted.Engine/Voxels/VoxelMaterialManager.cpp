// ReCrafted (c) 2016-2018 Always Too Late

#include "VoxelMaterialManager.h"
#include "Assets/VoxelMaterial.h"

SINGLETON_IMPL(VoxelMaterialManager)

void VoxelMaterialManager::OnInit()
{
}

void VoxelMaterialManager::OnDispose()
{
}

void VoxelMaterialManager::Update()
{
}

void VoxelMaterialManager::RegisterMaterial(VoxelMaterial* voxelMaterial)
{
    // Lock
    ScopeLock(m_materialLock);

    ASSERT(voxelMaterial);
    cvar voxelId = voxelMaterial->GetMaterial();
    ASSERT(m_materials.contains(voxelId) == false);

    // Set
    m_materials[voxelId] = voxelMaterial;
}

void VoxelMaterialManager::UnregisterMaterial(VoxelMaterial* voxelMaterial)
{
    // Lock
    ScopeLock(m_materialLock);

    ASSERT(voxelMaterial);
    cvar voxelId = voxelMaterial->GetMaterial();
    ASSERT(m_materials.contains(voxelId));

    // Try to remove from list
    cvar it = m_materials.find(voxelId);
    if(it != m_materials.end())
        m_materials.erase(it);
}

VoxelMaterial* VoxelMaterialManager::GetMaterial(const VoxelMaterial_t id)
{
    DEBUG_ASSERT(m_instance);

    // Lock
    rvar lock = m_instance->m_materialLock;
    ScopeLock(lock);

    if (!m_instance->m_materials.contains(id))
        return nullptr;

    return m_instance->m_materials[id];
}
