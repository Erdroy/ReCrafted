// ReCrafted (c) 2016-2018 Always Too Late

#include "SpaceObjectOctreeNode.h"
#include "SpaceObjectOctree.h"
#include "SpaceObjectChunk.h"
#include "SpaceObjectManager.h"
#include "SpaceObjectTables.hpp"
#include "Graphics/Camera.h"

#define HAS_LOCAL_NEIGH(id, dir) LocalNeighTable[id] & (1 << dir)

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

bool SpaceObjectOctreeNode::isChildrenProcessing() const
{
	if (m_processing)
		return true;

	if (m_populated) {
		for (auto i = 0; i < 8; i++)
		{
			if (m_childrenNodes[i]->isChildrenProcessing())
				return true;
		}
	}

	return false;
}

void SpaceObjectOctreeNode::markProcessing()
{
	m_processing = true;

	if (!m_populated)
		return;

	for (auto i = 0; i < 8; i++)
		m_childrenNodes[i]->markProcessing();
}

void SpaceObjectOctreeNode::worker_populate(IVoxelMesher* mesher)
{
	// WARNING: this function is called on WORKER THREAD!

	cvar childrenSize = m_size / 2;
    var boundsSize = Vector3::one() * static_cast<float>(childrenSize); 
    for (auto i = 0; i < 8; i++)
	{
		auto position = m_position + ChildrenNodeOffsets[i] * static_cast<float>(childrenSize);

		// construct node
		m_childrenNodes[i] = new SpaceObjectOctreeNode();
		m_childrenNodes[i]->m_position = position;
		m_childrenNodes[i]->m_size = childrenSize;
        m_childrenNodes[i]->m_bounds = BoundingBox(position, boundsSize);

		// set owner, parent and root
		m_childrenNodes[i]->owner = owner;
		m_childrenNodes[i]->parent = this;
		m_childrenNodes[i]->root = root;


		// set node id
		m_childrenNodes[i]->m_nodeId = i;

		// call event
		m_childrenNodes[i]->worker_generate(mesher);
	}
}

void SpaceObjectOctreeNode::worker_depopulate(IVoxelMesher* mesher)
{
	// WARNING: this function is called on WORKER THREAD!
	
	worker_generate(mesher);
}

void SpaceObjectOctreeNode::worker_generate(IVoxelMesher* mesher)
{
	// create chunk
	m_chunk = std::make_shared<SpaceObjectChunk>();
	m_chunk->init(this, owner->spaceObject);
	m_chunk->generate(mesher);
}

void SpaceObjectOctreeNode::populate()
{
	if (m_processing)
		return;

	if (m_populated)
		return;

	if (m_size <= MinimumNodeSize)
		return;

	m_processing = true;

	// queue populate job
	SpaceObjectManager::enqueue(this, ProcessMode::Populate, MakeDelegate(SpaceObjectOctreeNode::onPopulate));
}

void SpaceObjectOctreeNode::depopulate()
{
	if (m_processing)
		return;

	if (!m_populated)
		return;

	if (isChildrenProcessing())
		return;

	m_processing = true;

	// queue depopulate job
	SpaceObjectManager::enqueue(this, ProcessMode::Depopulate, MakeDelegate(SpaceObjectOctreeNode::onDepopulate));
}

void SpaceObjectOctreeNode::update()
{
	// update children when there is any
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

	// update chunk if there is one
	if (m_chunk)
		m_chunk->update();
}

void SpaceObjectOctreeNode::updateViews(Array<Vector3>& views)
{
    const float viewRangeMultiplier = 4.0f;

	if (m_processing)
		return;

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

	// do not populate/depopulate root
	if (m_isRoot)
		return;

	// X - node position
	// C - view position
	// A - populate range
	// B - depopulate range
	// P - outer bounds, actually no value for this, 
	// this is only for simplified version of the algorithm.
	// 
	// dist(X, A) = node_size + node_size * 0.5f
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

	// fast exit
	if(views.count() == 0)
	{
		// IF (there is no C's) AND populated: depopulate - otherwise: ignore.
		if (m_populated)
			depopulate();
		return;
	}

	// dist(X, A) = node_size + node_size * 0.5f
	auto distXA = m_size + m_size * 0.5f * viewRangeMultiplier;

	// dist(X, B) = dist(X, A) + node_size * 0.25
	auto distXB = distXA + m_size * 0.25f * viewRangeMultiplier;

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
		if (m_populated)
			depopulate();
		return;
	}

	// IF (there is no C's) AND populated: depopulate - otherwise: ignore.
	if (m_populated && views.count() > 0)
		depopulate();
}

void SpaceObjectOctreeNode::draw()
{
    //if (!Camera::getMainCamera()->getBoundingFrustum().contains(m_bounds))
    //    return;

	if(m_chunk)
	{
		m_chunk->draw();
		return;
	}

	if (!m_populated)
		return;

	for (auto i = 0; i < 8; i++)
	{
		auto node = m_childrenNodes[i];

		if (node)
		{
			node->draw();
		}
	}
}

void SpaceObjectOctreeNode::dispose()
{
	if (!m_populated)
	{
		// draw chunk if exists
		if(m_chunk)
			m_chunk->dispose();

		return;
	}

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

SpaceObjectOctreeNode* SpaceObjectOctreeNode::getNeighNode(NodeDirection::_enum direction) const
{
	// calculate target position
	var targetPosition = m_position + DirectionOffset[direction] * float(m_size);
	var targetLod = m_size;

	// traverse from root to the same target lod as this node
    for (var i = 0; i < owner->m_rootNodesCount; i++)
    {
        var node = owner->m_rootNodes[i]->findNode(targetPosition, targetLod);;

        if (node)
            return node;
    }

    return nullptr;
}

SpaceObjectOctreeNode* SpaceObjectOctreeNode::findNode(Vector3 position, int size)
{
	if (m_size == size)
	{
		// do not search anymore, because we found the exact node!
		return this;
	}

	if (!m_populated)
	{
		// uhh, this is lower-lod node, client will not be happy with this, or will be?
		return this;
	}

	// NOTE: we don't have to check 'equality' of position because 'size' does this
	var xSign = position.x > m_position.x;
	var ySign = position.y > m_position.y;
	var zSign = position.z > m_position.z;

	var caseCode = (zSign ? 1 : 0) | (ySign ? 1 : 0) << 1 | (xSign ? 1 : 0) << 2;
	var childId = NodeDirIds[caseCode];
	
	var node = m_childrenNodes[childId];

	// go further
	return node->findNode(position, size);
}

void SpaceObjectOctreeNode::onCreate()
{
	// upload the chunk
	if(m_chunk)
		m_chunk->upload();

}

void SpaceObjectOctreeNode::onDestroy()
{
	// destroy chunk
	SafeDispose(m_chunk);
}

void SpaceObjectOctreeNode::onPopulate()
{
	m_processing = false;
	m_populated = true;

	for(var i = 0; i < 8; i ++)
		m_childrenNodes[i]->onCreate();

	onDestroy();
}

void SpaceObjectOctreeNode::onDepopulate()
{
	m_populated = false;
	m_processing = false;

	// destroy all children
	for (auto i = 0; i < 8; i++)
	{
		auto node = m_childrenNodes[i];

		if (node)
		{
			// call event
			node->onDestroy();

			// delete node
			delete node;

			// cleanup
			m_childrenNodes[i] = nullptr;
		}
	}

	onCreate();
}
