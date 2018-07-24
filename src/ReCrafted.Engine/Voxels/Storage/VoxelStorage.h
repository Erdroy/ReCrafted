// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef VOXELSTORAGE_H
#define VOXELSTORAGE_H

// includes
#include "ReCrafted.h"

#include "Core/Streams/FileStream.h"

#include "Voxels/SpaceObject.h"
#include "VoxelChunkData.h"
#include "VoxelChunkCache.h"
#include "VoxelStorageHeader.h"

#include <spp.h>

class VoxelStorage : IDisposable
{
    friend class SpaceObject;

private:
    SpaceObject* spaceObject = nullptr;
    SpaceObjectSettings* settings = nullptr;

private:
    RefPtr<VoxelChunkCache> m_chunkCache = nullptr;

    FileStream* m_vxhStream = nullptr;
    VoxelStorageHeader* m_vxh = nullptr;

private:
    void LoadHeader();
    void SaveHeader();

public:
    void Init(SpaceObjectSettings* settings);
    void Update();
    void Dispose() override;

public:
    RefPtr<VoxelChunkData> CreateChunkData(const Vector3& nodePosition, int nodeSize, int nodeDepth);
    RefPtr<VoxelChunkData> GetChunkData(const Vector3& nodePosition);
    void ReadChunkData(const RefPtr<VoxelChunkData>& chunkData);
    void WriteChunkData(RefPtr<VoxelChunkData> chunkData);
    void FreeChunkData(RefPtr<VoxelChunkData> chunkData);
    void ReleaseChunkData(RefPtr<VoxelChunkData> chunkData);
};

#endif // VOXELSTORAGE_H
