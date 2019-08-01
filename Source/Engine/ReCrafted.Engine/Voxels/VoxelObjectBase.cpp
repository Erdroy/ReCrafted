// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelObjectBase.h"

#include "Content/ContentManager.h"
#include "Profiler/Profiler.h"
#include "Voxels/VoxelObjectManager.h"
#include "Voxels/Generator/VoxelGenerator.h"

void VoxelObjectBase::OctreeInitializeTask::Execute(void* userData)
{
    // Initialize octree
    Octree->Initialize();
}

void VoxelObjectBase::OctreeInitializeTask::Finish()
{
    // Rebuild the root nodes
    Octree->RebuildRootNodes();

    // Register object
    VoxelObjectManager::RegisterVoxelObject(VoxelObject);

    // Set voxel object's loading as false.
    VoxelObject->m_initialLoading = false;
}

VoxelObjectBase::VoxelObjectBase()
{
    m_octree = new VoxelObjectOctree(this);
}

VoxelObjectBase::~VoxelObjectBase()
{
    delete m_generator;
    delete m_storage;
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

    // Initialize components
    m_storage = new VoxelStorage(this);
    m_generator = new VoxelGenerator(this);

    // Load generator
    m_generator->Load();
}

void VoxelObjectBase::Update()
{
    CPU_PROFILE_FUNCTION(0);

    // TODO: Update views

    // Update octree
    m_octree->Update();

    // Update voxel storage
    m_storage->Update();
}

void VoxelObjectBase::DebugDraw()
{
    m_octree->DebugDraw();
}

void VoxelObjectBase::Modify(VoxelMaterial_t material, VoxelEditMode mode, const Vector3d& position, float size)
{
    
}
