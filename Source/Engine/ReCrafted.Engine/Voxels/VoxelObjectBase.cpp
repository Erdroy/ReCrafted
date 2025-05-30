// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelObjectBase.h"

#include "Content/ContentManager.h"
#include "Profiler/Profiler.h"
#include "Voxels/VoxelObjectManager.h"
#include "Voxels/Generator/VoxelGenerator.h"
#include "Core/Time.h"
#include "Core/Actors/ActorBase.h"
#include "Physics/RigidBodyActor.h"

#pragma region OctreeInitializeTask

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

#pragma endregion

#pragma region OctreeViewUpdateTask

void VoxelObjectBase::OctreeViewUpdateTask::UpdateNode(VoxelObjectOctree::Node* node)
{
    const auto viewRangeMultiplier = 4.0f;

    if (node->Size() == VoxelObjectOctree::Node::MinimumNodeSize)
        return;

    if (node->IsProcessing())
        return;

    if(node->IsPopulated())
    {
        const auto childrenNodes = node->ChildrenNodes();

        for(auto i = 0; i < VoxelObjectOctree::Node::ChildrenNodeCount; i ++)
        {
            const auto children = childrenNodes[i];
            
            if (!children)
                continue;

            UpdateNode(children);
        }
    }

    // we cannot depopulate/populate etc. because this node has populated children
    // which should be depopulated first, but this doesn't happen this time
    if (node->IsPopulated() && node->HasPopulatedChildren())
        return;

    // X - node position
    // C - view position
    // A - populate range
    // B - depopulate range
    // P - outer bounds, actually no value for this, 
    // this is only for simplified version of the algorithm.
    // 
    // dist(X, A) = node_size + node_size * 0.5f
    // dist(X, B) = dist(X, A) + node_size * 0.25
    // 
    // X ---C--|----|------- P
    //         A    B
    // X --- dist(A, C) ---> P
    // 
    // Algorithm:
    // IF (any C is in X-A) AND NOT populated: populate - otherwise: go further
    // IF (all C's are in B-P) AND populated: depopulate - otherwise: go further
    // IF (there is no C's) AND populated: depopulate - otherwise: ignore.

    // dist(X, A) = node_size + node_size * 0.5f
    const auto distXA = float(node->Size()) + float(node->Size()) * 0.5f * viewRangeMultiplier;

    // dist(X, B) = dist(X, A) + node_size * 0.25
    const auto distXB = distXA + float(node->Size()) * 0.25f * viewRangeMultiplier;

    // flags
    auto hasXA = false;
    auto hasXB = false;

    // check view flags
    for (auto&& point : Points)
    {
        const auto p = node->Position();
        const auto distanceXC = Vector3::Distance({ float(p.x), float(p.y), float(p.z) }, point);

        if (distanceXC <= distXA)
        {
            hasXA = true;
            break;
        }

        if (distanceXC <= distXB)
            hasXB = true;
    }

    // IF (any C is in X-A) AND NOT populated: populate - otherwise: go further
    if (hasXA)
    {
        if (!node->IsPopulated())
            NodesToPopulate.Add(node);
        return;
    }

    // IF (all C's are in B-P) AND populated: depopulate - otherwise: go further
    if (!hasXA && !hasXB)
    {
        if (node->IsPopulated() && !node->IsProcessing() && !node->AreChildrenProcessing())
            NodesToDepopulate.Add(node);
        return;
    }

    // IF (there is no C's) AND populated: depopulate - otherwise: ignore.
    if (node->IsPopulated() && !node->IsProcessing() && !node->AreChildrenProcessing())
        NodesToDepopulate.Add(node);
}

void VoxelObjectBase::OctreeViewUpdateTask::Execute(void* userData)
{
    const auto rootNodes = Octree->RootNodes();

    for (auto i = 0; i < Octree->RootNodesCount(); i++)
    {
        const auto rootNode = rootNodes[i];
        UpdateNode(rootNode);
    }
}

void VoxelObjectBase::OctreeViewUpdateTask::Finish()
{
    for(auto && node : NodesToPopulate)
    {
        if (!node || node->IsProcessing())
            continue;

        node->Populate();
    }

    for (auto&& node : NodesToDepopulate)
    {
        if (!node || node->IsProcessing() || node->AreChildrenProcessing())
            continue;

        node->Depopulate();
    }
}

#pragma endregion

void VoxelObjectBase::UpdateViews()
{
    if (m_lastViewUpdate + m_viewUpdateFrequency < Time::CurrentTime())
    {
        if(!m_viewActors.Empty())
        {
            // Queue new octree view update task
            const auto task = new OctreeViewUpdateTask();
            task->VoxelObject = this;
            task->Octree = m_octree;

            for (auto& viewActor : m_viewActors)
                task->Points.Add(viewActor->Position());

            Task::CreateTask(task, nullptr)->Queue();
        }

        m_lastViewUpdate = Time::CurrentTime();
    }
}

VoxelObjectBase::VoxelObjectBase()
{
    m_octree = new VoxelObjectOctree(this);
}

VoxelObjectBase::~VoxelObjectBase()
{
    // Wait for the VoxelObjectManager to end it's work
    VoxelObjectManager::GetInstance()->WaitForFinish();

    // Now we're safe to destroy, but unregister this VoxelObject at first.
    VoxelObjectManager::UnregisterVoxelObject(this);

    Destroy(m_rigidBodyActor);

    delete m_octree;
    delete m_storage;
    delete m_generator;
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

    // Create physics actor for this voxel object
    // this is the object that will get the collision attached
    m_rigidBodyActor = RigidBodyActor::CreateStatic();
    m_rigidBodyActor->Position(Vector3::Zero);

    m_lastViewUpdate = 0.0f;
}

void VoxelObjectBase::Update()
{
    CPU_PROFILE_FUNCTION(0);

    UpdateViews();

    // Update octree
    m_octree->Update();

    // Update voxel storage
    m_storage->Update();
}

void VoxelObjectBase::AddViewActor(ActorBase* viewActor)
{
    ASSERT(m_viewActors.Contains(viewActor) == false);
    m_viewActors.Add(viewActor);
}

void VoxelObjectBase::RemoveViewActor(ActorBase* viewActor)
{
    ASSERT(m_viewActors.Contains(viewActor) == true);
    m_viewActors.Remove(viewActor);
}

void VoxelObjectBase::Modify(const VoxelMaterial_t material, const VoxelEditMode mode, const Vector3& position, const float size)
{
    const auto boundingBox = BoundingBoxD(Vector3d(position.x, position.y, position.z), Vector3d::One * size * 2.5f);
   
    List<VoxelObjectOctree::Node*> nodes;
    m_octree->FindIntersecting(nodes, boundingBox);

    for (auto&& node : nodes)
    {
        if(node->IsProcessing())
        {
            Logger::LogWarning("Tried to modify VoxelChunk while it was still rebuilding.");
            continue;
        }

        // Do not modify when node don't have chunk
        if (!node->HasChunk()) continue;

        // Modify this node
        if (node->Modify(material, mode, position, size))
        {
            // Queue current node to rebuild
            node->Rebuild();
        }
    }
}
