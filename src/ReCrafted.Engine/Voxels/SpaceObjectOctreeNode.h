// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SPACEOBJECTOCTREENODE_H
#define SPACEOBJECTOCTREENODE_H

// includes
#include "ReCrafted.h"
#include "Core/Delegate.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/BoundingBox.h"
#include "Core/Containers/Array.h"
#include "Storage/VoxelEditMode.h"

struct IVoxelMesher;
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
class SpaceObjectOctreeNode
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
    RefPtr<SpaceObjectChunk> m_chunk = nullptr;

    bool m_populated = false;
    bool m_processing = false;
    bool m_rebuilding = false;
    bool m_isRoot = false;

private:
    bool HasPopulatedChildren();
    bool IsChildrenProcessing() const;
    void MarkProcessing();

    void CreateChunk(IVoxelMesher* mesher);

    void WorkerPopulate(IVoxelMesher* mesher);
    void WorkerDepopulate(IVoxelMesher* mesher);
    void WorkerRebuild(IVoxelMesher* mesher);

    void OnUpdate();
    void OnCreate();
    void OnRebuild();
    void OnDestroy();

    void OnPopulate();
    void OnDepopulate();

    void FindIntersecting(Array<SpaceObjectOctreeNode*>& nodes, BoundingBox& box, const int targetNodeSize = 16);
    SpaceObjectOctreeNode* GetNeighNode(NodeDirection::_enum direction) const;
    SpaceObjectOctreeNode* FindNode(Vector3 position, int size);

public:
    SpaceObjectOctreeNode()
    {
    }

public:
    void Update();
    void UpdateViews(Array<Vector3>& views);
    void Draw();
    void Dispose();

public:
    void Populate();
    void Depopulate();
    void Rebuild();

public:
    void Modify(VoxelEditMode::_enum mode, Vector3& position, float size);

public:
    SpaceObjectChunk* GetChunk() const
    {
        return m_chunk.get();
    }

public:
PROPERTY(int, Size) = 0;
PROPERTY(int, Depth) = 0;
PROPERTY(Vector3, Position) = {}; // NOTE: center position
PROPERTY(BoundingBox, Bounds) = {};
};

#endif // SPACEOBJECTOCTREENODE_H
