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
    var nodes = m_octree->findIntersecting(boundingBox, false); // NOTE: this will give us all type of LoD levels

    for(var && node : nodes)
    {
        // do not modify when node don't have chunk
        if (node->getChunk() == nullptr)
            continue;

        // modify this node
        node->modify(mode, position, size);

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
