// ReCrafted © 2016-2017 Always Too Late

#include "SpaceObject.h"

void SpaceObject::init()
{
	// create octree instance
	m_octree = std::make_shared<SpaceObjectOctree>();

	// build base node(s)
	m_octree->init(m_radius);

}

void SpaceObject::update()
{
	m_octree->update();
}

void SpaceObject::draw()
{

}

void SpaceObject::dispose()
{

}

void SpaceObject::updateViewPoint(Vector3& view)
{

}

Ptr<SpaceObject> SpaceObject::createSpaceObject()
{
	return std::make_shared<SpaceObject>();
}
