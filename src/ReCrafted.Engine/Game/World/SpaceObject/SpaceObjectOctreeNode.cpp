// ReCrafted (c) 2016-2018 Always Too Late

#include "SpaceObjectOctreeNode.h"
#include "SpaceObjectOctree.h"
#include "SpaceObjectChunk.h"
#include "SpaceObjectManager.h"
#include "SpaceObjectTables.hpp"
#include "Graphics/Camera.h"
#include "Utilities/VoxelUtils.h"

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
            cvar node = m_childrenNodes[i];
			
            if (!node)
                continue;
		    
		    if (node->isChildrenProcessing())
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

void SpaceObjectOctreeNode::createChunk(IVoxelMesher* mesher)
{
    // create chunk
    m_chunk = std::make_shared<SpaceObjectChunk>();
    m_chunk->init(this, owner->spaceObject);
    m_chunk->generate(mesher);
}

void SpaceObjectOctreeNode::worker_populate(IVoxelMesher* mesher) // WARNING: this function is called on WORKER THREAD!
{
	cvar childrenSize = m_size / 2;
    var boundsSize = Vector3::one() * static_cast<float>(childrenSize); 
    for (auto i = 0; i < 8; i++)
	{
		auto position = m_position + ChildrenNodeOffsets[i] * static_cast<float>(childrenSize);

		// construct node
		cvar childNode = m_childrenNodes[i] = new SpaceObjectOctreeNode();
        childNode->m_position = position;
        childNode->m_size = childrenSize;
        childNode->m_bounds = BoundingBox(position, boundsSize);

		// set owner, parent and root
        childNode->owner = owner;
        childNode->parent = this;
        childNode->root = root;

		// set node id
        childNode->m_childrenId = i;

		// generate chunk
        childNode->createChunk(mesher);
	}
}

void SpaceObjectOctreeNode::worker_depopulate(IVoxelMesher* mesher) // WARNING: this function is called on WORKER THREAD!
{
	
}

void SpaceObjectOctreeNode::worker_rebuild(IVoxelMesher* mesher) // WARNING: this function is called on WORKER THREAD!
{
    // rebuild chunks mesh
    m_chunk->rebuild(mesher);
}

void SpaceObjectOctreeNode::findIntersecting(Array<SpaceObjectOctreeNode*>& nodes, BoundingBox& box, const int targetNodeSize)
{
    // iterate all children nodes
    for (var i = 0; i < 8; i++)
    {
        cvar node = m_childrenNodes[i];

        if(!node)
            continue;

        if (BoundingBox::intersects(node->m_bounds, box))
        {
            if (targetNodeSize == 0)
            {
                node->findIntersecting(nodes, box, targetNodeSize);
                nodes.add(node);
            }
            else if(node->m_size == targetNodeSize)
            {
                nodes.add(node);
            }
            else
            {
                node->findIntersecting(nodes, box, targetNodeSize);
            }
        }
    }

}

void SpaceObjectOctreeNode::populate()
{
	if (m_processing || m_populated || m_size <= MinimumNodeSize)
		return;

	m_processing = true;

	// queue populate job
	SpaceObjectManager::enqueue(this, ProcessMode::Populate, MakeDelegate(SpaceObjectOctreeNode::onPopulate));
}

void SpaceObjectOctreeNode::depopulate()
{
	if (m_processing || !m_populated || isChildrenProcessing())
		return;

	// queue depopulate job
	SpaceObjectManager::enqueue(this, ProcessMode::Depopulate, MakeDelegate(SpaceObjectOctreeNode::onDepopulate));
}

void SpaceObjectOctreeNode::rebuild()
{
    if (m_processing || m_rebuilding)
        return;

    m_rebuilding = true;
    
    // queue rebuild job
    SpaceObjectManager::enqueue(this, ProcessMode::Rebuild, MakeDelegate(SpaceObjectOctreeNode::onRebuild));
}

void SpaceObjectOctreeNode::modify(VoxelEditMode::_enum mode, Vector3& position, float size)
{
    cvar chunk = getChunk();

    if (chunk == nullptr)
        return;

    cvar chunkData = chunk->getChunkData();
    cvar voxels = chunkData->getData();
    cvar chunkScale = static_cast<float>(chunkData->getLod());

    for (var x = 0; x < VoxelChunkData::ChunkDataSize; x++)
    for (var y = 0; y < VoxelChunkData::ChunkDataSize; y++)
    for (var z = 0; z < VoxelChunkData::ChunkDataSize; z++)
    {
        var point = Vector3(float(x), float(y), float(z)) * chunkScale + chunkData->getChunkPosition();

        cvar currentValue = voxels[INDEX_3D(x, y, z, VoxelChunkData::ChunkDataSize)];
        cvar distance = Vector3::distance(position, point);

        if (distance <= size + 0.5f)
        {
            var value = size - distance;
            var newValue = VOXEL_FROM_FLOAT(value);

            if (mode == VoxelEditMode::Additive)
            {
                newValue = -newValue;

                if (newValue < currentValue)
                    voxels[INDEX_3D(x, y, z, VoxelChunkData::ChunkDataSize)] = newValue;
            }
            else
            {
                if (newValue > currentValue)
                    voxels[INDEX_3D(x, y, z, VoxelChunkData::ChunkDataSize)] = newValue;
            }
        }
    }
}

void SpaceObjectOctreeNode::onUpdate()
{
}

void SpaceObjectOctreeNode::onCreate()
{
    m_processing = false;

    // upload chunk if needed
    if (m_chunk->needsUpload())
        m_chunk->upload();
}

void SpaceObjectOctreeNode::onRebuild()
{
    m_rebuilding = false;

    // upload chunk if needed
    if (m_chunk->needsUpload())
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

    for (var i = 0; i < 8; i++)
    {
        cvar node = m_childrenNodes[i];

        if (node)
            node->onCreate();
    }

    //onDestroy();
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

    onUpdate();
}

void SpaceObjectOctreeNode::updateViews(Array<Vector3>& views)
{
    cvar viewRangeMultiplier = 4.0f;

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
	cvar distXA = m_size + m_size * 0.5f * viewRangeMultiplier;

	// dist(X, B) = dist(X, A) + node_size * 0.25
	cvar distXB = distXA + m_size * 0.25f * viewRangeMultiplier;

	// flags
	var hasXA = false;
	var hasXB = false;

	// check view flags
	for (auto && view : views)
	{
		cvar distanceXC = Vector3::distance(m_position, view);

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
    if (!Camera::getMainCamera()->getBoundingFrustum().contains(m_bounds))
        return;

	if(m_chunk && !m_populated)
	{
		m_chunk->draw();
		return;
	}

	for (var i = 0; i < 8; i++)
	{
		var node = m_childrenNodes[i];

		if (node)
		{
			node->draw();
		}
	}
}

void SpaceObjectOctreeNode::dispose()
{
    // draw chunk if exists
    if (m_chunk)
        m_chunk->dispose();

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
