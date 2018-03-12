// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef VOXELCHUNKDATA_H
#define VOXELCHUNKDATA_H

// includes
#include "ReCrafted.h"

class VoxelChunkData
{
    friend class VoxelStorage;

public:
    static const int ChunkSize = 16; // 16 'cubes' on signle axis
    static const int ChunkDataSize = ChunkSize + 1; // 17 corners on signle axis (cubes_count + 1)

private:
    sbyte* m_voxelData = nullptr;
    int m_size = 0;
    bool m_loaded = false;
    uint64_t m_id = 0u;
    Vector3 m_position = {};

public:
    VoxelChunkData()
    {
        m_voxelData = new sbyte[ChunkDataSize * ChunkDataSize * ChunkDataSize];
    }

    ~VoxelChunkData()
    {
        SafeDeleteArrayNN(m_voxelData);
    }

public:
    int getSize() const
    {
        return m_size;
    }

    sbyte* getData() const
    {
        return m_voxelData;
    }

    bool isLoaded() const
    {
        return m_loaded;
    }
};

#endif // VOXELCHUNKDATA_H
