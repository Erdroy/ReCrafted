// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef SPACEOBJECTCHUNK_H
#define SPACEOBJECTCHUNK_H

// includes
#include "Core/Math/Vector3.h"
#include "Core/Lock.h"
#include "Storage/VoxelChunkData.h"
#include <atomic>

struct IVoxelMesher;
class Mesh;
class SpaceObject;
class SpaceObjectOctreeNode;

class SpaceObjectChunk
{
    friend class SpaceObjectManager;

private:
    typedef enum
    {
        None,
        SwapMesh,
        UploadMesh,
        ClearMesh
    } UploadType;

private:
    SpaceObject* spaceObject = nullptr;
    SpaceObjectOctreeNode* node = nullptr;

    int m_lod = 0;
    uint64_t m_id = 0u;
    Vector3 m_position = {};

    RefPtr<VoxelChunkData> m_chunkData = {};
    RefPtr<Mesh> m_mesh = nullptr;
    RefPtr<Mesh> m_newMesh = nullptr;

    Lock m_uploadLock = {};
    std::atomic<UploadType> m_uploadType = None;

private:
    void SetUpload(RefPtr<Mesh> mesh, UploadType uploadType);

public:
    void Init(SpaceObjectOctreeNode* node, SpaceObject* spaceObject);
    void Upload();
    void Draw();
    void Dispose();

public:
    void Generate(IVoxelMesher* mesher);
    void Rebuild(IVoxelMesher* mesher);

    bool NeedsUpload()
    {
        return m_uploadType != None;
    }

    RefPtr<VoxelChunkData> GetChunkData() const
    {
        return m_chunkData;
    }

public:
    static uint64_t CalculateChunkId(const Vector3& position);
};

#endif // SPACEOBJECTCHUNK_H

