// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SPACEOBJECTCHUNK_H
#define SPACEOBJECTCHUNK_H

// includes
#include "Core/Math/Vector3.h"
#include "Core/Lock.h"
#include "Storage/VoxelChunkData.h"

struct IVoxelMesher;
class Mesh;
class SpaceObject;
class SpaceObjectOctreeNode;

class SpaceObjectChunk
{
    friend class SpaceObjectManager;

private:
    SpaceObject* spaceObject = nullptr;
    SpaceObjectOctreeNode* node = nullptr;

    int m_lod = 0;
    uint64_t m_id = 0u;
    Vector3 m_position = {};

    Ref<VoxelChunkData> m_chunkData = {};
    Ref<Mesh> m_mesh = nullptr;
    Ref<Mesh> m_newMesh = nullptr;

    Lock m_meshLock = {};

private:
    uint8_t getLodBorders();

public:
    void init(SpaceObjectOctreeNode* node, SpaceObject* spaceObject);
    void upload();
    void draw();
    void dispose();

public:
    void generate(IVoxelMesher* mesher);
    void rebuild(IVoxelMesher* mesher);

    bool needsUpload() const
    {
        return m_newMesh != nullptr;
    }

    Ref<VoxelChunkData> getChunkData() const
    {
        return m_chunkData;
    }

public:
    static uint64_t calculateChunkId(const Vector3& position);
};

#endif // SPACEOBJECTCHUNK_H
