// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelGenerator.h"

#include "Core/Heightmap.h"
#include "Voxels/Voxel.h"
#include "Voxels/VoxelObjectBase.h"
#include "Voxels/Assets/VoxelObjectAsset.h"


    /**
     * \brief Maps given point to CHM UV coordinates in ranges [0.0 incl, 1.0 excl].
     * \param face The face which will get mapped point.
     * \param point The point which will be mapped.
     * \return The UV coordinates in ranges [0.0 incl, 1.0 excl].
     */
FORCEINLINE static Vector2 GetTexcoord(const int face, const Vector3d& point)
{
    Vector2 texcoord;

    switch (face)
    {
    case 0:
    {
        const auto localPoint = point * (1.0f / fabs(point.z));
        texcoord.y = localPoint.y;
        texcoord.x = -localPoint.x;
        break;
    }
    case 1:
    {
        const auto localPoint = point * (1.0f / fabs(point.z));
        texcoord.y = localPoint.y;
        texcoord.x = localPoint.x;
        break;
    }
    case 2:
    {
        const auto localPoint = point * (1.0f / fabs(point.x));
        texcoord.y = localPoint.y;
        texcoord.x = -localPoint.z;
        break;
    }
    case 3:
    {
        const auto localPoint = point * (1.0f / fabs(point.x));
        texcoord.y = localPoint.y;
        texcoord.x = localPoint.z;
        break;
    }
    case 4:
    {
        const auto localPoint = point * (1.0f / fabs(point.y));
        texcoord.y = localPoint.z;
        texcoord.x = localPoint.x;
        break;
    }
    case 5:
    {
        const auto localPoint = point * (1.0f / fabs(point.y));
        texcoord.y = -localPoint.z;
        texcoord.x = localPoint.x;
        break;
    }
    default: throw;
    }

    // clamp
    texcoord.x = (texcoord.x + 1.0f) * 0.5f;
    texcoord.y = 1.0f - (texcoord.y + 1.0f) * 0.5f;

    if (texcoord.x >= 1.0f)
        texcoord.x = 0.99999f;

    if (texcoord.y >= 1.0f)
        texcoord.y = 0.99999f;

    return texcoord;
}

/**
* \brief Selects the sphere face is used by the point.
* \param point The point which will be check.
* \return The selected face.
*/
FORCEINLINE static int GetFace(const Vector3d& point)
{
    const auto absPoint = Vector3d::Abs(const_cast<Vector3d&>(point));

    if (absPoint.x > absPoint.y)
    {
        if (absPoint.x > absPoint.z)
        {
            if (point.x > 0.0f)
            {
                return 3; // right
            }
            return 2; // left
        }

        if (point.z > 0.0f)
        {
            return 0; // front
        }
        return 1; // back
    }

    if (absPoint.y > absPoint.z)
    {
        if (point.y > 0.0f)
        {
            return 4; // up
        }

        return 5; // down
    }

    if (point.z > 0.0f)
    {
        return 0; // front
    }
    return 1; // back
}

VoxelGenerator::~VoxelGenerator()
{
    //SafeDispose(m_bitmap);
}

Voxel VoxelGenerator::GenerateFromCHM(const Vector3d& origin, const Vector3d& position,
    const int mipLevel, const int lodSize, const int radius, const int height) const
{
    if (position.LengthSquared() == 0)
        return Voxel::Default;

    const auto lod = float(lodSize);

    // Get sphere face
    const auto sphereFace = GetFace(position);

    // Map spherecoords
    const auto texcoord = GetTexcoord(sphereFace, position);

    const auto heightmap = m_voxelObject->Asset()->GetHeightmap();
    const auto sample = heightmap->SampleCube8BitBilinear(Heightmap::Top, Vector2(texcoord.x, texcoord.y), 0);

    // the terrain height (over planet, sphere is the base)
    const auto terrainHeight = float(radius) + (float(sample) / 256.0f) * float(height);

    // the height over sphere
    const auto currentHeight = static_cast<float>((position - origin).Length());
    const auto heightDiff = currentHeight - terrainHeight;

    // calculate voxel value
    const auto voxelValue = heightDiff / lod;

    auto voxel = Voxel::Create(voxelValue, 0u);

    const auto surfaceLevel = fabs(heightDiff);

    if (surfaceLevel <= lod * 4.0f)
        voxel.material = 2u;

    if (surfaceLevel <= lod * 1.25f)
        voxel.material = 1u;

    // convert voxel value to voxel
    return voxel; // TODO: Read material id from CHM
}

void VoxelGenerator::Load()
{
    //ASSERT(m_bitmap == nullptr);
    //m_bitmap = CHMBitmap::CreateFromFile(settings->fileName);
}

void VoxelGenerator::Unload()
{
    //SafeDispose(m_bitmap);
}

inline void SetVoxel(Voxel* voxelData, const int x, const int y, const int z, const Voxel voxel)
{
    voxelData[Math::Index3(
        x + VoxelChunkData::EnableNormalCorrection,
        y + VoxelChunkData::EnableNormalCorrection,
        z + VoxelChunkData::EnableNormalCorrection,
        VoxelChunkData::ChunkDataSize)] = voxel;
}

bool VoxelGenerator::GenerateChunkData(const RefPtr<VoxelChunkData>& chunk, const Vector3d& position, const int lod, const int depth)
{
    const int voxelCount = VoxelChunkData::ChunkDataSize * VoxelChunkData::ChunkDataSize * VoxelChunkData::ChunkDataSize;
    Voxel voxels[voxelCount]; // Well, stack will cry, but game will be still happy

    // TODO: Allocate aligned data structure for `voxels`

    const auto octree = m_voxelObject->Octree();

    const auto asset = m_voxelObject->Asset();
    const auto voxelSize = static_cast<float>(lod);
    const auto chunkPosition = m_voxelObject->Position();
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
                const auto offset = Vector3d(double(x), double(y), double(z));
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
