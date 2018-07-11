// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef VOXELCHUNKDATA_H
#define VOXELCHUNKDATA_H

// includes
#include "ReCrafted.h"
#include "Core/Math/Vector3.h"

class VoxelChunkData
{
    friend class VoxelStorage;
    friend class VoxelGenerator;
    friend class VoxelChunkCache;

public:
    static const int ChunkSize = 16; // 16 'cubes' on signle axis
    static const int ChunkDataSize = ChunkSize + 1; // 17 corners on signle axis (cubes_count + 1)

private:
    sbyte* m_voxelData = nullptr;
    Vector3 m_nodePosition = {};
    Vector3 m_chunkPosition = {};
    uint64_t m_id = 0u;
    int m_nodeDepth = 0;
    int m_size = 0;
    bool m_loaded = false;
    bool m_hasSurface = false;
    bool m_isFilled = false;

    double m_timeCached = 0.0;
    bool m_cached = false;

public:
    VoxelChunkData() = default;

    ~VoxelChunkData()
    {
        DeallocateData();
    }

public:
    void AllocateData()
    {
        ASSERT(m_voxelData == nullptr);

        cvar size = ChunkDataSize * ChunkDataSize * ChunkDataSize;
        m_voxelData = new sbyte[size];

        memset(m_voxelData, m_isFilled ? -127 : 127, size);
    }

    void DeallocateData()
    {
        SafeDeleteArray(m_voxelData);
    }

public:
    int GetSize() const
    {
        return m_size;
    }

    int GetLod() const
    {
        return m_size / ChunkSize;
    }

    sbyte* GetData() const
    {
        return m_voxelData;
    }

    Vector3 GetNodePosition() const
    {
        return m_nodePosition;
    }

    Vector3 GetChunkPosition() const
    {
        return m_chunkPosition;
    }

    bool IsLoaded() const
    {
        return m_loaded;
    }

    bool IsFilled() const
    {
        return m_isFilled;
    }

    void IsFilled(bool isFilled)
    {
        m_isFilled = isFilled;
    }

    bool HasData() const
    {
        return m_voxelData != nullptr;
    }

    bool HasSurface() const
    {
        return m_hasSurface;
    }

    void HasSurface(bool value)
    {
        m_hasSurface = value;
    }
};

#endif // VOXELCHUNKDATA_H
