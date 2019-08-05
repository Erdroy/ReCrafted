// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Voxels/Voxel.h"

class VoxelChunkData
{
    DELETE_COPY_MOVE(VoxelChunkData)

    friend class VoxelStorage;
    friend class VoxelGenerator;
    friend class VoxelChunkCache;

public:
    /// <summary>
    ///     Enables or disables normal correction.
    /// </summary>
    static const bool EnableNormalCorrection = true;

public:
    /// <summary>
    ///     ChunkSize - 16 'cubes' on single axis.
    /// </summary>
    static const int ChunkSize = 16;

    /// <summary>
    ///     ChunkDataLength - 19 'cubes' on single axis (cubes_count + 1) - [|the normal range| and 16],
    ///     or when NormalCorrection is enabled - 19 corners on single axis(cubes_count + 3) - [-1, | the normal range | , 16 and 17].
    /// </summary>
    static const int ChunkDataSize = EnableNormalCorrection ? ChunkSize + 3 : ChunkSize + 1;

    /// <summary>
    ///     ChunkDataLength - 17 'cubes' on single axis (cubes_count + 1) - [|the normal range| and 16], 
    ///     or when NormalCorrection is enabled - 19 corners on single axis(cubes_count + 3) - [-1, | the normal range | , 16 and 17].
    /// </summary>
    static const int ChunkDataLength = EnableNormalCorrection ? ChunkSize + 2 : ChunkSize + 1;

    /// <summary>
    ///      The chunk data start. It is - 1 when NormalCorrection is enabled otherwise it is just 0.
    /// </summary>
    static const int ChunkDataStart = EnableNormalCorrection ? -1 : 0;

private:
    Voxel* m_voxelData = nullptr;
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

        const auto size = ChunkDataSize * ChunkDataSize * ChunkDataSize;
        m_voxelData = new Voxel[size]; // TODO: Alloc aligned

        auto fill = m_isFilled ? Voxel::Full : Voxel::Empty;
        memset(m_voxelData, *reinterpret_cast<uint32_t*>(&fill), size * sizeof(Voxel));
    }

    void DeallocateData()
    {
        if (m_voxelData == nullptr)
            return;

        delete[] m_voxelData;
        m_voxelData = nullptr;
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

    Voxel* GetData() const
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

    void IsFilled(const bool isFilled)
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

    void HasSurface(const bool value)
    {
        m_hasSurface = value;
    }

    void ResetCache()
    {
        m_cached = false;
    }

    FORCEINLINE Voxel GetVoxel(const int x, const int y, const int z) const
    {
        return GetVoxel(m_voxelData, x, y, z);
    }

    FORCEINLINE void SetVoxel(const int x, const int y, const int z, const Voxel voxel) const
    {
        SetVoxel(m_voxelData, x, y, z, voxel);
    }

public:
    FORCEINLINE static Voxel GetVoxel(Voxel* voxelData, const int x, const int y, const int z)
    {
        return voxelData[Math::Index3(
            x + EnableNormalCorrection,
            y + EnableNormalCorrection,
            z + EnableNormalCorrection,
            ChunkDataSize)];
    }

    FORCEINLINE static void SetVoxel(Voxel* voxelData, const int x, const int y, const int z, const Voxel voxel)
    {
        voxelData[Math::Index3(
            x + EnableNormalCorrection,
            y + EnableNormalCorrection,
            z + EnableNormalCorrection,
            ChunkDataSize)] = voxel;
    }
};
