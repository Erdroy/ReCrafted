// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/Lock.h"
#include "Voxels/VoxelChunkData.h"

class VoxelGenerator
{
    DELETE_COPY_MOVE(VoxelGenerator);

private:
    VoxelObjectBase* m_voxelObject = nullptr;
    Lock m_loadLock = {};
    bool m_loaded = false;

public:
    explicit VoxelGenerator(VoxelObjectBase* voxelObject) : m_voxelObject(voxelObject) {}
    ~VoxelGenerator();

private:
    FORCE_INLINE Voxel GenerateFromCHM(const Vector3d& origin, const Vector3d& position, int mipLevel, int lodSize, int radius, int height) const;

public:
    void Load();
    void Unload();

    bool IsLoaded()
    {
        ScopeLock(m_loadLock);
        return m_loaded;
    }

public:
    bool GenerateChunkData(const RefPtr<VoxelChunkData>& chunk, const Vector3d& position, int lod, int depth);
};
