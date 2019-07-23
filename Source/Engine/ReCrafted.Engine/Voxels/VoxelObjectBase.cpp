// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelObjectBase.h"
#include "Profiler/Profiler.h"
#include "VoxelObjectManager.h"
#include "Content/ContentManager.h"

void VoxelObjectBase::OctreeInitializeTask::Execute(void* userData)
{
    // Initialize octree
    Octree->Initialize();
}

void VoxelObjectBase::OctreeInitializeTask::Finish()
{
    // Register object
    VoxelObjectManager::RegisterVoxelObject(VoxelObject);
}

VoxelObjectBase::VoxelObjectBase()
{
    m_octree = new VoxelObjectOctree(this);
}

VoxelObjectBase::~VoxelObjectBase()
{
    delete m_octree;

    // Unregister object
    VoxelObjectManager::UnregisterVoxelObject(this);
}

void VoxelObjectBase::Initialize()
{
    // Start octree initialization task
    const auto task = new OctreeInitializeTask();
    task->VoxelObject = this;
    task->Octree = m_octree;
    Task::CreateTask(task, nullptr)->Queue();
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
