// ReCrafted © 2016-2017 Always Too Late

#include "SpaceObjectOctree.h"
#include "Core/Math/Color.h"
#include "Graphics/DebugDraw.h"
#include "SpaceObject.h"

Vector3 childrenNodeOffsets[8] = {
	Vector3(-0.5f,  0.5f,  0.5f),
	Vector3( 0.5f,  0.5f,  0.5f),
	Vector3( 0.5f,  0.5f, -0.5f),
	Vector3(-0.5f,  0.5f, -0.5f),

	Vector3(-0.5f, -0.5f,  0.5f),
	Vector3( 0.5f, -0.5f,  0.5f),
	Vector3( 0.5f, -0.5f, -0.5f),
	Vector3(-0.5f, -0.5f, -0.5f)
};

bool SpaceObjectOctreeNode::hasPopulatedChildren()
{
	for (auto i = 0; i < 8; i++)
	{
		auto node = m_childrenNodes[i];

		if (node && node->m_populated)
			return true;
	}

	return false;
}

void SpaceObjectOctreeNode::populate()
{
	if (m_size <= MinimumNodeSize)
		return;

	// construct children nodes
	auto childrenSize = m_size / 2;
	for (auto i = 0; i < 8; i++)
	{
		if (m_childrenNodes[i])
			throw; // WTF?

		auto position = m_position + childrenNodeOffsets[i] * static_cast<float>(childrenSize);

		// construct node
		m_childrenNodes[i] = new SpaceObjectOctreeNode(position, childrenSize);

		// set owner
		m_childrenNodes[i]->owner = owner;

		// call event
		m_childrenNodes[i]->onCreate();
	}

	m_populated = true;

	// call event
	onPopulate();
}

void SpaceObjectOctreeNode::depopulate()
{
	if (!m_populated)
		return;

	if (hasPopulatedChildren())
		throw;

	for(auto i = 0; i < 8; i ++)
	{
		auto node = m_childrenNodes[i];

		if(node)
		{
			// call event
			node->onDestroy();

			// delete node
			delete node;

			// cleanup
			m_childrenNodes[i] = nullptr;
		}
	}

	m_populated = false;

	// call event
	onDepopulate();
}

void SpaceObjectOctreeNode::update()
{
	if(m_populated)
	{
		for (auto i = 0; i < 8; i++)
		{
			auto node = m_childrenNodes[i];

			if (node)
				node->update();
		}

		return;
	}

	auto size = Vector3::one() * static_cast<float>(m_size);

	DebugDraw::setColor(Color(255, 105, 0));
	DebugDraw::drawWireCube(m_position, size);
	DebugDraw::setColor(Color(0.6f, 0.35f, 0.0f, 0.2f));
	DebugDraw::drawCube(m_position, size);
}

void SpaceObjectOctreeNode::updateViews(Array<Vector3>& views)
{
	if(m_populated)
	{
		// update children node views, because we are populated
		for (auto i = 0; i < 8; i++)
		{
			auto node = m_childrenNodes[i];

			if (node)
				node->updateViews(views);
		}
	}

	// we cannot depopulate/populate etc. because this node has populated children
	// which should be depoplated first, but this doesn't happen this time
	if (m_populated && hasPopulatedChildren())
		return;

	// X - node position
	// C - view position
	// A - populate range
	// B - depopulate range
	// P - outer bounds, actually no value for this, 
	// this is only for simplified version of the algorithm.
	// 
	// dist(X, A) = node_size + node_size * 0.25
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

	// dist(X, A) = node_size + node_size * 0.25
	auto distXA = m_size + m_size * 0.25f;

	// dist(X, B) = dist(X, A) + node_size * 0.25
	auto distXB = distXA + m_size * 0.25f;

	// flags
	auto hasXA = false;
	auto hasXB = false;

	// check view flags
	for (auto && view : views)
	{
		auto distanceXC = Vector3::distance(m_position, view);

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
		if (!m_populated)
			populate();
		return;
	}

	// IF (all C's are in B-P) AND populated: depopulate - otherwise: go further
	if (!hasXA && !hasXB)
	{
		if (!m_populated)
			depopulate();
		return;
	}

	// IF (there is no C's) AND populated: depopulate - otherwise: ignore.
	if (m_populated)
		depopulate();
}

void SpaceObjectOctreeNode::dispose()
{
	if (!m_populated)
		return;

	for (auto i = 0; i < 8; i++)
	{
		auto node = m_childrenNodes[i];

		if (node)
		{
			// dispose
			node->dispose();

			// delete node
			delete node;

			// cleanup
			m_childrenNodes[i] = nullptr;
		}
	}
}

void SpaceObjectOctreeNode::onCreate()
{
}

void SpaceObjectOctreeNode::onDestroy()
{
}

void SpaceObjectOctreeNode::onPopulate()
{
}

void SpaceObjectOctreeNode::onDepopulate()
{
}
