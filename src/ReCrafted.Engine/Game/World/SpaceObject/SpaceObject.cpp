// ReCrafted (c) 2016-2018 Always Too Late

#include "SpaceObject.h"
#include "Core/Math/Math.h"
#include "Storage/VoxelStorage.h"
#include "Common/Profiler/Profiler.h"
#include "SpaceObjectChunk.h"
#include "Utilities/VoxelUtils.h"

void SpaceObject::init(SpaceObjectSettings& settings)
{
    // initialize voxel storage
    m_voxelStorage = std::make_shared<VoxelStorage>();
    m_voxelStorage->init(settings);
    m_voxelStorage->spaceObject = this;

	// set settings
	m_settings = settings;

	// create octree instance
	m_octree = std::make_shared<SpaceObjectOctree>();
	m_octree->spaceObject = this;

	set_position(settings.position);

	// build base node(s)
	m_octree->init();
}

void SpaceObject::update()
{
	m_octree->update();
	m_octree->updateViews(m_views);

	// clear views for this frame
	m_views.clear();
}

void SpaceObject::draw()
{
	m_octree->draw();
}

void SpaceObject::dispose()
{
    SafeDispose(m_octree);
    SafeDispose(m_voxelStorage);
}

void SpaceObject::updateViewPoint(Vector3& view)
{
	// add view
	m_views.add(view);
}

void SpaceObject::modify(VoxelEditMode::_enum mode, Vector3& position, float size)
{
    var bbSize = Vector3(size, size, size) * 2.0f;
    bbSize.x = ceilf(bbSize.x);
    bbSize.y = ceilf(bbSize.y);
    bbSize.z = ceilf(bbSize.z);

    var boundingBox = BoundingBox(position, bbSize);
    var nodes = m_octree->findIntersecting(boundingBox, true); // NOTE: this will give us all LoD levels, but we need only the LoD-0.

    for(var && node : nodes)
    {
        cvar chunk = node->getChunk();

        if (chunk == nullptr)
            continue;

        cvar chunkData = chunk->getChunkData();
        cvar voxels = chunkData->getData();
        cvar chunkScale = static_cast<float>(chunkData->getSize()) / static_cast<float>(VoxelChunkData::ChunkSize);

        for (var x = 0; x < VoxelChunkData::ChunkDataSize; x++)
        for (var y = 0; y < VoxelChunkData::ChunkDataSize; y++)
        for (var z = 0; z < VoxelChunkData::ChunkDataSize; z++)
        {
            var point = Vector3(float(x), float(y), float(z)) * chunkScale + chunkData->getChunkPosition();

            cvar currentValue = voxels[INDEX_3D(x, y, z, VoxelChunkData::ChunkDataSize)];
            cvar distance = Vector3::distance(position, point);

            if (distance <= size + 0.5f)
            {
                var value = size - distance;
                var newValue = VOXEL_FROM_FLOAT(value);

                if (mode == VoxelEditMode::Additive)
                {
                    newValue = -newValue;

                    if (newValue < currentValue)
                        voxels[INDEX_3D(x, y, z, VoxelChunkData::ChunkDataSize)] = newValue;
                }
                else
                {
                    if (newValue > currentValue)
                        voxels[INDEX_3D(x, y, z, VoxelChunkData::ChunkDataSize)] = newValue;
                }
            }
        }

        // queue current node to rebuild
        node->rebuild();
    }
}

SpaceObjectSettings& SpaceObject::getSettings()
{
	return m_settings;
}

Ptr<SpaceObject> SpaceObject::createSpaceObject(SpaceObjectSettings& settings)
{
    var spaceObject = std::make_shared<SpaceObject>();
    spaceObject->init(settings);
    return spaceObject;
}
