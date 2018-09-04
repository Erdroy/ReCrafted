// ReCrafted (c) 2016-2018 Always Too Late

#include "VoxelGenerator.h"
#include "CHMBitmap.h"
#include "CHMHelpers.h"
#include "Core/Math/Math.h"
#include "Voxels/SpaceObject.h"
#include "Voxels/SpaceObjectTables.hpp"
#include "Voxels/Storage/VoxelChunkData.h"
#include "Voxels/Utilities/VoxelUtils.h"

Voxel VoxelGenerator::GenerateFromCHM(const Vector3& origin, const Vector3& position,
    const int mipLevel, const int lodSize, const int radius, const int height) const
{
    if (position.LengthSqr() == 0)
        return Voxel::Default;

    // Get sphere face
    cvar sphereFace = CHMHelpers::GetFace(position);

    // Map spherecoords
    cvar texcoord = CHMHelpers::GetTexcoord(sphereFace, position);

    // Sample CHM
    cvar sample = m_bitmap->Sample(static_cast<CHMFace::_enum>(sphereFace), texcoord.x, texcoord.y, mipLevel) / 255.0f;

    // the terrain height (over planet, sphere is the base)
    cvar terrainHeight = radius + sample * height;

    // the height over sphere
    cvar currentHeight = (position - origin).Length();
    cvar heightDiff = currentHeight - terrainHeight;

    // calculate voxel value
    cvar voxelValue = (heightDiff) / lodSize;

    var voxel = Voxel::Create(voxelValue, 0u, 255u);

    //if (voxel.value == Voxel::FullValue)
    //    voxel.material = 1u;

    // convert voxel value to voxel
    return voxel; // TODO: Read material id from CHM
}

void VoxelGenerator::Init(SpaceObjectSettings* settings)
{
    this->settings = settings;
}

void VoxelGenerator::Load()
{
    ASSERT(m_bitmap == nullptr);
    m_bitmap = CHMBitmap::CreateFromFile(settings->fileName);
}

void VoxelGenerator::Unload()
{
    SafeDispose(m_bitmap);
}

void VoxelGenerator::Dispose()
{
    SafeDispose(m_bitmap);
}

inline void SetVoxel(Voxel* voxelData, int x, int y, int z, Voxel voxel)
{
    voxelData[INDEX_3D(
        x + VoxelChunkData::EnableNormalCorrection,
        y + VoxelChunkData::EnableNormalCorrection,
        z + VoxelChunkData::EnableNormalCorrection,
        VoxelChunkData::ChunkDataSize)] = voxel;
}

bool VoxelGenerator::GenerateChunkData(const RefPtr<VoxelChunkData>& chunk, const Vector3& position, int lod, int depth)
{
    const int voxelCount = VoxelChunkData::ChunkDataSize * VoxelChunkData::ChunkDataSize * VoxelChunkData::ChunkDataSize;
    Voxel voxels[voxelCount]; // Well, stack will cry, but game will be still happy

    cvar octree = spaceObject->GetOctree();

    cvar voxelSize = static_cast<float>(lod);
    cvar lodSize = LoDTable[lod];

    cvar chunkPosition = spaceObject->GetPosition();

    cvar mipLevel = Math::Clamp(octree->GetMaxDepth() - depth, 0, m_bitmap->GetLoDCount());

    var hasSurface = false;
    sbyte lastVoxel = 0;

    // Temporary TODO: Generate proper materials
    chunk->AddMaterial(MaterialSet_t{ { 1, 2, 1 },{ 1, 2, 1 }, 0, 0 });
    chunk->AddMaterial(MaterialSet_t{ { 1, 1, 1 },{ 1, 2, 1 }, 0, 0 }); // 1
    chunk->AddMaterial(MaterialSet_t{ { 2, 2, 2 },{ 1, 2, 1 }, 0, 0 }); // 2
    chunk->AddMaterial(MaterialSet_t{ { 3, 3, 3 },{ 1, 2, 1 }, 0, 0 }); // 3

    var voxelVolumeSign = 0;
    for (var x = VoxelChunkData::ChunkDataStart; x < VoxelChunkData::ChunkDataLength; x++)
    {
        for (var y = VoxelChunkData::ChunkDataStart; y < VoxelChunkData::ChunkDataLength; y++)
        {
            for (var z = VoxelChunkData::ChunkDataStart; z < VoxelChunkData::ChunkDataLength; z++)
            {
                cvar offset = Vector3(float(x), float(y), float(z));
                cvar voxelPosition = position + offset * voxelSize;

                cvar value = GenerateFromCHM(chunkPosition, voxelPosition, mipLevel, lodSize,
                    settings->minSurfaceHeight,
                    settings->hillsHeight);

                // Surface checking
                if(!hasSurface && lastVoxel != value.value)
                    hasSurface = true;

                lastVoxel = value.value;
                voxelVolumeSign += value.value;

                SetVoxel(voxels, x, y, z, value);
            }
        }
    }

    // Check sign count
    cvar isUnderground = voxelVolumeSign < 0;
    chunk->IsFilled(isUnderground);

    // If this chunk has any surface, then we will need to allocate data and copy the temporary array
    if(hasSurface)
    {
        if(!chunk->HasData())
            chunk->AllocateData();

        cvar chunkData = chunk->GetData();
        cvar chunkDataSize = voxelCount * sizeof(Voxel);
        memcpy_s(chunkData, chunkDataSize, voxels, chunkDataSize);
    }

    return hasSurface;
}
