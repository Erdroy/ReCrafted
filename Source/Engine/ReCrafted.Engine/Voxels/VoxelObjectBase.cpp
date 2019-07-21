// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelObjectBase.h"
#include "Profiler/Profiler.h"
#include "Rendering/Debug/DebugDraw.h"

VoxelObjectBase::VoxelObjectBase()
{
    m_octree = new VoxelObjectOctree(this);
}

VoxelObjectBase::~VoxelObjectBase()
{
    delete m_octree;
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
