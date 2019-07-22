// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelObjectBase.h"
#include "Profiler/Profiler.h"
#include "VoxelObjectManager.h"
#include "Content/ContentManager.h"

VoxelObjectBase::VoxelObjectBase()
{
    // Register object
    VoxelObjectManager::RegisterVoxelObject(this);

    m_octree = new VoxelObjectOctree(this);
}

VoxelObjectBase::~VoxelObjectBase()
{
    delete m_octree;

    // Unregister object
    VoxelObjectManager::UnregisterVoxelObject(this);

    // Destroy asset
    Destroy(m_asset);
}

void VoxelObjectBase::Initialize()
{
    m_octree->Initialize();
}

void VoxelObjectBase::Load(const char* assetFile)
{
    // Load voxel asset
    m_asset = ContentManager::LoadAsset<VoxelObjectAsset>(assetFile);
}

void VoxelObjectBase::Update()
{
    CPU_PROFILE_FUNCTION(0);

    // TODO: Update views

    // Update octree
    m_octree->Update();
}

void VoxelObjectBase::Modify(VoxelMaterial_t material, VoxelEditMode mode, const Vector3d& position, float size)
{
    
}
