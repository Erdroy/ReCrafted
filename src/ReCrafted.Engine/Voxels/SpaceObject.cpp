// ReCrafted (c) 2016-2018 Always Too Late

#include "SpaceObject.h"
#include "Common/Profiler/Profiler.h"
#include "Core/TaskManager.h"
#include "Core/Math/Math.h"
#include "Storage/VoxelStorage.h"
#include "Generator/VoxelGenerator.h"

void SpaceObject::Init(SpaceObjectSettings& settings)
{
    // set settings
    m_settings = settings;

    // initialize voxel generator
    m_generator = std::make_shared<VoxelGenerator>();
    m_generator->spaceObject = this;
    m_generator->Init(&m_settings);

    // initialize voxel storage
    m_voxelStorage = std::make_shared<VoxelStorage>();
    m_voxelStorage->spaceObject = this;
    m_voxelStorage->Init(&m_settings);

    // create octree instance
    m_octree = std::make_shared<SpaceObjectOctree>();
    m_octree->spaceObject = this;

    // Set current position
    SetPosition(m_settings.position);
}

void SpaceObject::GeneratePrimary()
{
    m_generator->Load();
    m_octree->GeneratePrimary();
}

void SpaceObject::Update()
{
    m_octree->Update();
    m_octree->UpdateViews(m_views);

    // clear views for this frame
    m_views.Clear();
}

void SpaceObject::Draw()
{
    m_octree->Draw();
}

void SpaceObject::Dispose()
{
    SafeDispose(m_octree);
    SafeDispose(m_voxelStorage);
}

void SpaceObject::UpdateViewPoint(Vector3& view)
{
    // add view
    m_views.Add(view);
}

void SpaceObject::Modify(VoxelEditMode::_enum mode, Vector3& position, float size)
{
    var bbSize = Vector3(size, size, size) * 2.0f;
    bbSize.x = ceilf(bbSize.x);
    bbSize.y = ceilf(bbSize.y);
    bbSize.z = ceilf(bbSize.z);

    var boundingBox = BoundingBox(position, bbSize);
    var nodes = m_octree->FindIntersecting(boundingBox, false); // NOTE: this will give us all type of LoD levels

    for (var&& node : nodes)
    {
        // do not modify when node don't have chunk
        if (node->GetChunk() == nullptr)
            continue;

        // modify this node
        node->Modify(mode, position, size);

        // queue current node to rebuild
        node->Rebuild();
    }
}

SpaceObjectSettings& SpaceObject::GetSettings()
{
    return m_settings;
}

RefPtr<SpaceObject> SpaceObject::CreateSpaceObject(SpaceObjectSettings& settings)
{
    var spaceObject = std::make_shared<SpaceObject>();
    spaceObject->Init(settings);
    return spaceObject;
}
