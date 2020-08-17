// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelGenerator.h"

#include "Core/Heightmap.h"
#include "Voxels/Voxel.h"
#include "Voxels/VoxelObjectBase.h"
#include "Voxels/Assets/VoxelObjectAsset.h"

// original source: https://en.wikipedia.org/wiki/Cube_mapping

inline Vector2 MapCubeUV(const Heightmap::CubeFace face, const Vector3& point)
{
    Vector2 texcoord;

    switch (face)
    {
    case Heightmap::CubeFace::Front:
    {
        const auto localPoint = point * (1.0f / fabs(point.z));
        texcoord.y = localPoint.y;
        texcoord.x = -localPoint.x;
        break;
    }
    case Heightmap::CubeFace::Back:
    {
        const auto localPoint = point * (1.0f / fabs(point.z));
        texcoord.y = localPoint.y;
        texcoord.x = localPoint.x;
        break;
    }
    case Heightmap::CubeFace::Left:
    {
        const auto localPoint = point * (1.0f / fabs(point.x));
        texcoord.y = localPoint.y;
        texcoord.x = -localPoint.z;
        break;
    }
    case Heightmap::CubeFace::Right:
    {
        const auto localPoint = point * (1.0f / fabs(point.x));
        texcoord.y = localPoint.y;
        texcoord.x = localPoint.z;
        break;
    }
    case Heightmap::CubeFace::Top:
    {
        const auto localPoint = point * (1.0f / fabs(point.y));
        texcoord.y = localPoint.z;
        texcoord.x = localPoint.x;
        break;
    }
    case Heightmap::CubeFace::Bottom:
    {
        const auto localPoint = point * (1.0f / fabs(point.y));
        texcoord.y = -localPoint.z;
        texcoord.x = localPoint.x;
        break;
    }
    default: throw;
    }

    // Clamp
    texcoord.x = (texcoord.x + 1.0f) * 0.5f;
    texcoord.y = 1.0f - (texcoord.y + 1.0f) * 0.5f;

    return texcoord;
}

inline Heightmap::CubeFace MapCubeFace(const Vector3& point)
{
    const auto absPoint = Vector3::Abs(point);

    if (absPoint.x > absPoint.y)
    {
        if (absPoint.x > absPoint.z)
            return point.x > 0.0f ? Heightmap::CubeFace::Right : Heightmap::CubeFace::Left;

        return point.z > 0.0f ? Heightmap::CubeFace::Front : Heightmap::CubeFace::Back;
    }

    if (absPoint.y > absPoint.z)
        return point.y > 0.0f ? Heightmap::CubeFace::Top : Heightmap::CubeFace::Bottom;

    return point.z > 0.0f ? Heightmap::CubeFace::Front : Heightmap::CubeFace::Back;
}

Voxel VoxelGenerator::GenerateFromCHM(const Vector3& origin, const Vector3& position,
    const int mipLevel, const int lodSize, const int radius, const int height) const
{
    if (position.LengthSquared() == 0)
        return Voxel::Default;

    const auto lod = float(lodSize);

    // Get sphere face
    const auto sphereFace = MapCubeFace(position);

    // Map to sphere coords
    const auto texcoord = MapCubeUV(sphereFace, position);

    const auto heightmap = m_voxelObject->Asset()->GetHeightmap();
    const auto sample = heightmap->SampleCube8BitBilinear(sphereFace, texcoord, mipLevel);

    // the terrain height (over planet, sphere is the base)
    const auto terrainHeight = float(radius) + float(sample) / 255.0f * float(height);

    // the height over sphere
    const auto currentHeight = static_cast<float>((position - origin).Length());
    const auto heightDiff = currentHeight - terrainHeight;

    // calculate voxel value
    const auto voxelValue = heightDiff / lod;

    auto voxel = Voxel::Create(voxelValue, 1u);

    const auto surfaceLevel = fabs(heightDiff);

    if (surfaceLevel <= lod * 4.0f)
        voxel.material = 3u;

    if (surfaceLevel <= lod * 1.25f)
        voxel.material = 2u;

    // convert voxel value to voxel
    return voxel; // TODO: Read material id from CHM
}

inline void SetVoxel(Voxel* voxelData, const int x, const int y, const int z, const Voxel voxel)
{
    voxelData[Math::Index3(
        x + VoxelChunkData::EnableNormalCorrection,
        y + VoxelChunkData::EnableNormalCorrection,
        z + VoxelChunkData::EnableNormalCorrection,
        VoxelChunkData::ChunkDataSize)] = voxel;
}

bool VoxelGenerator::GenerateChunkData(const RefPtr<VoxelChunkData>& chunk, const Vector3& position, const int lod, const int depth)
{
    const int voxelCount = VoxelChunkData::ChunkDataSize * VoxelChunkData::ChunkDataSize * VoxelChunkData::ChunkDataSize;
    Voxel voxels[voxelCount]; // Well, stack will cry, but game will be still happy

    // TODO: Allocate aligned data structure for `voxels`

    const auto octree = m_voxelObject->Octree();

    const auto asset = m_voxelObject->Asset();
    const auto voxelSize = static_cast<float>(lod);
    const auto chunkPosition = Vector3(
        float(m_voxelObject->Position().x), 
        float(m_voxelObject->Position().y), 
        float(m_voxelObject->Position().z)
    );
    const auto mipLevel = 0;// Math::Clamp(octree->GetMaxDepth() - depth, 0, m_bitmap->GetLoDCount());

    auto hasSurface = false;
    sbyte lastVoxel = 0;

    auto voxelVolumeSign = 0;
    for (auto x = VoxelChunkData::ChunkDataStart; x < VoxelChunkData::ChunkDataLength; x++)
    {
        for (auto y = VoxelChunkData::ChunkDataStart; y < VoxelChunkData::ChunkDataLength; y++)
        {
            for (auto z = VoxelChunkData::ChunkDataStart; z < VoxelChunkData::ChunkDataLength; z++)
            {
                const auto offset = Vector3(float(x), float(y), float(z));
                const auto voxelPosition = position + offset * voxelSize;

                const auto value = GenerateFromCHM(chunkPosition, voxelPosition, mipLevel, lod,
                    asset->MinimumSurfaceHeight(),
                    asset->HillsHeight());

                // Surface checking
                if (!hasSurface && lastVoxel != value.value)
                    hasSurface = true;

                lastVoxel = value.value;
                voxelVolumeSign += value.value;

                SetVoxel(voxels, x, y, z, value);
            }
        }
    }

    // Check sign count
    const auto isUnderground = voxelVolumeSign < 0;
    chunk->IsFilled(isUnderground);

    // If this chunk has any surface, then we will need to allocate data and copy the temporary array
    if (hasSurface)
    {
        if (!chunk->HasData())
            chunk->AllocateData();

        const auto chunkData = chunk->GetData();
        const auto chunkDataSize = voxelCount * sizeof(Voxel);
        memcpy_s(chunkData, chunkDataSize, voxels, chunkDataSize);
    }

    return hasSurface;
}
