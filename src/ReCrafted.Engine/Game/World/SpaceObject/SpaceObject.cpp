// ReCrafted © 2016-2017 Always Too Late

#include "SpaceObject.h"

void SpaceObject::init(Vector3 position, float radius)
{
	// create octree instance
	m_octree = std::make_shared<SpaceObjectOctree>();
	m_octree->spaceObject = this;
	m_octree->set_position(position);
	
	set_position(position);

	// build base node(s)
	m_octree->init(radius);
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
	m_octree->dispose();
}

void SpaceObject::updateViewPoint(Vector3& view)
{
	// add view
	m_views.add(view);
}

Ptr<SpaceObject> SpaceObject::createSpaceObject()
{
	return std::make_shared<SpaceObject>();
}
