// ReCrafted © 2016-2017 Always Too Late

#include "SpaceObjectChunk.h"
#include "SpaceObjectOctree.h"
#include "SpaceObject.h"
#include "Graphics/DebugDraw.h"

void SpaceObjectChunk::init(SpaceObjectOctreeNode* node, SpaceObject* spaceObject)
{
	this->spaceObject = spaceObject;
	this->node = node;

	m_objectNormal = spaceObject->get_position() - node->get_position();
	m_objectNormal.normalize();
}

void SpaceObjectChunk::generate()
{
	// TODO: generate voxel data
	// TODO: generate mesh
}

void SpaceObjectChunk::update()
{
	DebugDraw::setColor(Color(0, 255, 0));
	DebugDraw::drawLine(node->get_position(), node->get_position() + m_objectNormal * node->get_size() * 0.3f);
	DebugDraw::drawCube(node->get_position() + m_objectNormal * node->get_size() * 0.3f, Vector3::one());
}

void SpaceObjectChunk::draw()
{

}

void SpaceObjectChunk::dispose()
{
}
