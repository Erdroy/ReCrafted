// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef VOXELGENERATOR_H
#define VOXELGENERATOR_H

// includes
#include "ReCrafted.h"
#include "Core/Lock.h"
#include "Voxels/SpaceObject.h"
#include "CHMBitmap.h"
#include "Voxels/Storage/VoxelChunkData.h"

class VoxelGenerator : IDisposable
{
    friend class SpaceObject;

private:
    SpaceObject* spaceObject = nullptr;
    SpaceObjectSettings* settings = nullptr;

    RefPtr<CHMBitmap> m_bitmap = nullptr;
    Lock m_loadLock = {};
    bool m_loaded = false;

private:
    FORCEINLINE sbyte GenerateFromCHM(const Vector3& origin, const Vector3& position, int mipLevel, int lodSize, int radius, int height) const;

public:
    void Init(SpaceObjectSettings* settings);
    void Load();
    void Unload();
    void Dispose() override;

    bool IsLoaded()
    {
        ScopeLock(m_loadLock);
        return m_loaded;
    }

public:
    bool GenerateChunkData(const RefPtr<VoxelChunkData>& chunk, const Vector3& position, int lod, int depth);
};

#endif // VOXELGENERATOR_H
