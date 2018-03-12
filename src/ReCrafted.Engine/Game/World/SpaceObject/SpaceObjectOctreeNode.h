// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SPACEOBJECTOCTREENODE_H
#define SPACEOBJECTOCTREENODE_H

// includes
#include "Core/Delegate.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/BoundingBox.h"
#include "Core/Containers/Array.h"

interface IVoxelMesher;
class SpaceObjectOctree;
class SpaceObjectChunk;
class SpaceObject;

struct NodeDirection
{
	enum _enum : byte
	{
		Front = 0,
		Back,
		Left,
		Right,
		Top,
		Bottom
	};
};

/**
* \brief OctreeNode of the Octree of SpaceObject.
*/
class SpaceObjectOctreeNode : DelegateHandler
{
	friend class SpaceObject;
	friend class SpaceObjectOctree;
	friend class SpaceObjectManager;

public:
	static const int MinimumNodeSize = 16;

private:
	SpaceObjectOctree* owner = nullptr;
	SpaceObjectOctreeNode* parent = nullptr;
	SpaceObjectOctreeNode* root = nullptr;

    SpaceObjectOctreeNode* m_childrenNodes[8] = {};
	Ptr<SpaceObjectChunk> m_chunk = nullptr;

    bool m_populated = false;
    bool m_processing = false;
    bool m_isRoot = false;
    int m_childrenId = 0;

    uint64_t m_nodeId = 0;

private:
	bool hasPopulatedChildren();
	bool isChildrenProcessing() const;
	void markProcessing();

	void worker_populate(IVoxelMesher* mesher);
	void worker_depopulate(IVoxelMesher* mesher);
	void worker_generate(IVoxelMesher* mesher);

    void findIntersecting(Array<SpaceObjectOctreeNode*>& nodes, BoundingBox& box, const int targetNodeSize = 16);
    SpaceObjectOctreeNode* getNeighNode(NodeDirection::_enum direction) const;
    SpaceObjectOctreeNode* findNode(Vector3 position, int size);

public:
	SpaceObjectOctreeNode() {}

public:
	void populate();
	void depopulate();
	void update();
	void updateViews(Array<Vector3>& views);
	void draw();
	void dispose();

public:
	void onCreate();
	void onDestroy();

	void onPopulate();
	void onDepopulate();

public:
	PROPERTY(Vector3, position) = {}; // NOTE: center position
	PROPERTY(int, size) = {};
    PROPERTY(BoundingBox, bounds) = {};
};

#endif // SPACEOBJECTOCTREENODE_H
