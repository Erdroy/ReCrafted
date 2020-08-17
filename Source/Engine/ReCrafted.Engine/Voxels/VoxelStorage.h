// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Voxels/VoxelChunkData.h"

class VoxelObjectAsset;

class VoxelStorage final
{
    DELETE_COPY_MOVE(VoxelStorage);
    friend class VoxelObjectBase;

private:
    VoxelObjectBase* m_voxelObject = nullptr;

private:
    RefPtr<VoxelChunkCache> m_chunkCache = nullptr;

public:
    explicit VoxelStorage(VoxelObjectBase* voxelObject);
    ~VoxelStorage();

public:
    void Init(VoxelObjectAsset* asset);
    void Update();

public:
    RefPtr<VoxelChunkData> CreateChunkData(const Vector3& nodePosition, int nodeSize, int nodeDepth);
    RefPtr<VoxelChunkData> GetChunkData(const Vector3& nodePosition);
    void ReadChunkData(const RefPtr<VoxelChunkData>& chunkData);

    void WriteChunkData(const RefPtr<VoxelChunkData>& chunkData);
    void FreeChunkData(const RefPtr<VoxelChunkData>& chunkData);
    void ReleaseChunkData(const RefPtr<VoxelChunkData>& chunkData);
};
