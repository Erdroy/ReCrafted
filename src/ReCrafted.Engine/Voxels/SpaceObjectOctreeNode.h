// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SPACEOBJECTOCTREENODE_H
#define SPACEOBJECTOCTREENODE_H

// includes
#include "ReCrafted.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/BoundingBox.h"
#include "Core/Containers/Array.h"
#include "Storage/VoxelEditMode.h"
#include <atomic>

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

    int m_id = 0;

    SpaceObjectOctreeNode* m_childrenNodes[8] = {};
    RefPtr<SpaceObjectChunk> m_chunk = nullptr;

    bool m_isRoot = false;
    std::atomic<bool> m_populated = false;
    std::atomic<bool> m_processing = false;
    std::atomic<bool> m_rebuilding = false;

private:
    bool HasPopulatedChildren();
    bool IsProcessing() const;

    void CreateChunk();
    void GenerateChunk(IVoxelMesher* mesher);

    void WorkerPopulate(IVoxelMesher* mesher);
    void WorkerDepopulate(IVoxelMesher* mesher);
    void WorkerRebuild(IVoxelMesher* mesher);

    void DestroyChildren();

    void OnCreate();
    void OnRebuild();
    void OnDestroy();

    void OnPopulate();
    void OnDepopulate();

    void FindIntersecting(Array<SpaceObjectOctreeNode*>& nodes, const BoundingBox& box, int targetNodeSize = 16);
    SpaceObjectOctreeNode* FindNeighNode(NodeDirection::_enum direction) const;
    SpaceObjectOctreeNode* FindNode(const Vector3& position, int size);
    SpaceObjectOctreeNode* FindNode(const Vector3& position);

public:
    void UpdateViews(Array<Vector3>& views);
    void Dispose();

public:
    void Populate();
    void Depopulate();
    void Rebuild();

public:
    bool Modify(VoxelEditMode::_enum mode, Vector3& position, float size);

public:
    SpaceObjectChunk* GetChunk() const
    {
        return m_chunk.get();
    }

public:
PROPERTY(int, Size) = 0;
PROPERTY(int, Depth) = 0;
PROPERTY_REF(Vector3, Position) = {}; // NOTE: center position
PROPERTY_REF(BoundingBox, Bounds) = {};
};

#endif // SPACEOBJECTOCTREENODE_H
