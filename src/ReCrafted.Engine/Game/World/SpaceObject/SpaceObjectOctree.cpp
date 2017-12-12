// ReCrafted © 2016-2017 Always Too Late

#include "SpaceObjectOctree.h"
#include "SpaceObjectOctreeNode.h"
#include "Common/Profiler/Profiler.h"
#include "Core/Math/math.h"
#include "SpaceObject.h"

void SpaceObjectOctree::init()
{
	// calculate diameter
	var settings = spaceObject->getSettings();
	var radius = settings.minSurfaceHeight + settings.maxBuildHeight;
	var objectSize = Math::roundUpToPow2(static_cast<int>(radius * 2)); // diameter rounded up to power of 2

	// calculate bounds size
	var size = Vector3::one() * static_cast<float>(objectSize);

	// build bounds
	m_bounds = BoundingBox(settings.position, size);

	// create root node
	m_rootNode = new SpaceObjectOctreeNode();
	m_rootNode->set_position(settings.position);
	m_rootNode->set_size(objectSize);
	
	// set owner, parent and root
	m_rootNode->owner = this;
	m_rootNode->parent = nullptr;
	m_rootNode->root = m_rootNode;
	m_rootNode->m_isRoot = true;

	// populate root node
	m_rootNode->populate();
}

void SpaceObjectOctree::update()
{
	Profiler::beginProfile("SpaceObjectOctree::update");
	// do we need fixed update rate here?

	// update root node
	m_rootNode->update();
	Profiler::endProfile();
}

void SpaceObjectOctree::updateViews(Array<Vector3>& views)
{
	Profiler::beginProfile("SpaceObjectOctree::updateViews");
	m_rootNode->updateViews(views);
	Profiler::endProfile();
}

void SpaceObjectOctree::draw()
{
	Profiler::beginProfile("SpaceObjectOctree::draw");
	m_rootNode->draw();
	Profiler::endProfile();
}

void SpaceObjectOctree::dispose()
{
	SafeDispose(m_rootNode);
	SafeDelete(m_rootNode);
}
