// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Voxels/VoxelChunkData.h"

class VoxelGenerator
{
    DELETE_COPY_MOVE(VoxelGenerator);

private:
    VoxelObjectBase* m_voxelObject = nullptr;

public:
    explicit VoxelGenerator(VoxelObjectBase* voxelObject) : m_voxelObject(voxelObject) {}
    ~VoxelGenerator() = default;

private:
    FORCE_INLINE Voxel GenerateFromCHM(const Vector3& origin, const Vector3& position, int mipLevel, int lodSize, int radius, int height) const;

public:
    bool GenerateChunkData(const RefPtr<VoxelChunkData>& chunk, const Vector3& position, int lod, int depth);
};
