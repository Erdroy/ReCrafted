// ReCrafted © 2016-2017 Always Too Late

#include "SpaceObjectOctree.h"
#include "Core/Math/Color.h"
#include "Graphics/DebugDraw.h"

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

void SpaceObjectOctreeNode::populate()
{
	if (m_size <= MinimumNodeSize)
		return;

	m_populated = true;

	// construct children nodes
	auto childrenSize = m_size / 2;
	for (auto i = 0; i < 8; i++)
	{
		if (m_childrenNodes[i])
			throw; // WTF?

		auto position = m_position + childrenNodeOffsets[i] * static_cast<float>(childrenSize);

		// construct node
		m_childrenNodes[i] = new SpaceObjectOctreeNode(position, childrenSize);
	}

	// this node is successfully populated
}

void SpaceObjectOctreeNode::depopulate()
{
	if (!m_populated)
		return;

	for(auto i = 0; i < 8; i ++)
	{
		auto node = m_childrenNodes[i];

		if(node)
		{
			// depopulate all children-children nodes
			// it will depopulate children-children-children nodes etc. xD
			node->depopulate();

			// delete node
			delete node;

			// cleanup
			m_childrenNodes[i] = nullptr;
		}
	}

	m_populated = false;
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
	DebugDraw::setColor(Color(1.0f, 0.55f, 0.0f, 0.5f));
	DebugDraw::drawCube(m_position, size);
}

void SpaceObjectOctreeNode::dispose()
{
}