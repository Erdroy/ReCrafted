// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SPACEOBJECTOCTREENODE_H
#define SPACEOBJECTOCTREENODE_H

// includes
#include "ReCrafted.h"
#include "Core/Math/BoundingBox.h"
#include "Core/Containers/Array.h"
#include "Storage/VoxelEditMode.h"
#include <atomic>
#include "Voxel.h"

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

    SpaceObjectOctreeNode* m_neighborNodes[6] = {};
    SpaceObjectOctreeNode* m_childrenNodes[8] = {};

    int m_id = 0;
    bool m_isRoot = false;
    std::atomic<bool> m_populated = false;
    std::atomic<bool> m_processing = false;
    std::atomic<bool> m_forceRebuild = false;

    RefPtr<SpaceObjectChunk> m_chunk = nullptr;

private:
    bool HasPopulatedChildren();
    bool IsProcessing() const;

    void UpdateNeighborNodes();

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
    void Initialize();
    void Dispose();
    void DrawDebug();
    void UpdateViews(Array<Vector3>& views);
        
    FORCEINLINE void Update()
    {
        if(m_forceRebuild)
        {
            Rebuild();
            m_forceRebuild = false;
        }
    }

public:
    void Populate();
    void Depopulate();
    void Rebuild();

public:
    bool Modify(VoxelMaterial_t material, VoxelEditMode::_enum mode, const Vector3& position, float size);

public:
    RefPtr<SpaceObjectChunk> GetChunk() const
    {
        return m_chunk;
    }

    SpaceObjectOctreeNode* GetNeighbor(const NodeDirection::_enum direction) const
    {
        return m_neighborNodes[direction];
    }

    SpaceObjectOctreeNode* GetChildren(const int childrenId) const
    {
        ASSERT(childrenId >= 0 && childrenId < 8);

        if (!m_populated)
            return nullptr;

        return m_childrenNodes[childrenId];
    }

    bool HasChildren() const
    {
        return m_childrenNodes[0] != nullptr;
    }

    bool IsPopulated() const
    {
        return m_populated;
    }

public:
PROPERTY(int, Size) = 0;
PROPERTY(int, Depth) = 0;
PROPERTY_REF(Vector3, Position) = {}; // NOTE: center position
PROPERTY_REF(BoundingBox, Bounds) = {};
};

#endif // SPACEOBJECTOCTREENODE_H
