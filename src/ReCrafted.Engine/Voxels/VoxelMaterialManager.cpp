// ReCrafted (c) 2016-2018 Always Too Late

#include "VoxelMaterialManager.h"
#include "Assets/VoxelMaterial.h"

SINGLETON_IMPL(VoxelMaterialManager)

void VoxelMaterialManager::OnInit()
{
    // Load default material
    m_defaultMaterial = ContentManager::LoadAsset<VoxelMaterial>("VoxelMaterials/Default");

    // Set default material
    SetDefaultMaterial(m_defaultMaterial);

    // Temporary TODO: Move to game, when Content API will be done!
    AddMaterial(ContentManager::LoadAsset<VoxelMaterial>("VoxelMaterials/Rock"));
    AddMaterial(ContentManager::LoadAsset<VoxelMaterial>("VoxelMaterials/Rock1"));
    AddMaterial(ContentManager::LoadAsset<VoxelMaterial>("VoxelMaterials/Rock2"));
    AddMaterial(ContentManager::LoadAsset<VoxelMaterial>("VoxelMaterials/Grass"));
    AddMaterial(ContentManager::LoadAsset<VoxelMaterial>("VoxelMaterials/Soil"));
    AddMaterial(ContentManager::LoadAsset<VoxelMaterial>("VoxelMaterials/Clay"));
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

    // Try to remove from list
    cvar it = m_materials.find(voxelId);
    if(it != m_materials.end())
        m_materials.erase(it);
}

void VoxelMaterialManager::SetDefaultMaterial(VoxelMaterial* material)
{
    m_instance->m_defaultMaterial = material;
}

void VoxelMaterialManager::AddMaterial(VoxelMaterial* material)
{
    DEBUG_ASSERT(m_instance);

    // Lock
    rvar lock = m_instance->m_materialLock;
    ScopeLock(lock);

    // Add material
    m_instance->m_materials.insert(std::make_pair(material->GetMaterial(), material));
    m_instance->m_allMaterials.emplace_back(material);
}

VoxelMaterial* VoxelMaterialManager::GetMaterial(const VoxelMaterial_t id)
{
    DEBUG_ASSERT(m_instance);

    // Lock
    rvar lock = m_instance->m_materialLock;
    ScopeLock(lock);

    if (!m_instance->m_materials.contains(id))
    {
        // Return default material
        return m_instance->m_defaultMaterial;
    }

    return m_instance->m_materials[id];
}
