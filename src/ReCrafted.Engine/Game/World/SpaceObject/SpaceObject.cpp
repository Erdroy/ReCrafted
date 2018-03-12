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

void SpaceObject::modify(VoxelEditMode::_enum mode, VoxelEditShape::_enum shape, Vector3& position, float size)
{
    // TODO:
    // build bounding box
    // find intersecting chunks
    // get their LoD-0 data
    // apply modification
    // - how to apply this to all lod's?
    // - how to store/save it

    var bbSize = Vector3(size, size, size);
    var boundingBox = BoundingBox(position, bbSize);
    var toModify = m_octree->findIntersecting(boundingBox); // NOTE: this will give us all LoD levels, but we need only the LoD-0.

    for(var && node : toModify)
    {
        cvar chunk = node->getChunk();
        cvar chunkData = chunk->getChunkData();
        cvar voxels = chunkData->getData();
        cvar size = chunkData->getSize();

        // modify the data (this is in-place edit, so voxels are modified as we modify the array)
        for(var x = static_cast<int>(boundingBox.left() / size); x < boundingBox.right() / size; x ++)
        {
            for (var y = static_cast<int>(boundingBox.bottom() / size); y < boundingBox.top() / size; y++)
            {
                for (var z = static_cast<int>(boundingBox.back() / size); z < boundingBox.front() / size; z++)
                {

                    voxels[INDEX_3D(x, y, z, 17)] = 127;
                }
            }
        }

        m_voxelStorage->writeChunkData(chunkData);

        // TODO: apply on all higher-LoD nodes

        // write chunk as it is modified now
        //m_voxelStorage->writeChunk(chunkData, node->get_position(), node->get_size());

        // queue node to regenerate
        node->regenerate();
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
