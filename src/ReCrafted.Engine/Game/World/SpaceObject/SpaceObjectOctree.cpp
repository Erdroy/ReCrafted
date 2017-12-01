// ReCrafted © 2016-2017 Always Too Late

#include "SpaceObjectOctree.h"
#include "Core/Math/math.h"

void SpaceObjectOctree::init(float objectRadius)
{
	auto size = static_cast<int>(objectRadius) * 2;

	// TODO: check if it is power of two

	// calculate bounds size
	m_size = Vector3::one() * static_cast<float>(size);

	// build bounds
	m_bounds = BoundingBox(m_position, m_size);

	// create root node
	m_rootNode = new SpaceObjectOctreeNode(m_position, size);
	
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
	// do we need fixed update rate here?

	// update root node
	m_rootNode->update();
}

void SpaceObjectOctree::updateViews(Array<Vector3>& views)
{
	m_rootNode->updateViews(views);
}

void SpaceObjectOctree::draw()
{
	m_rootNode->draw();
}

void SpaceObjectOctree::dispose()
{
	SafeDispose(m_rootNode);
	SafeDelete(m_rootNode);
}
