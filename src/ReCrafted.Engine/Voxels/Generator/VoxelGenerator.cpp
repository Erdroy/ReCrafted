// ReCrafted (c) 2016-2018 Always Too Late

#include "VoxelGenerator.h"
#include "CHMBitmap.h"
#include "CHMHelpers.h"
#include "Core/Math/Math.h"
#include "Voxels/SpaceObject.h"
#include "Voxels/SpaceObjectTables.hpp"
#include "Voxels/Storage/VoxelChunkData.h"
#include "Voxels/Utilities/VoxelUtils.h"

sbyte VoxelGenerator::GenerateFromCHM(const Vector3& origin, const Vector3& position,
    const int mipLevel, const int lodSize, const int radius, const int height) const
{
    if (position.LengthSqr() == 0)
        return 0;

    // Get sphere face
    cvar sphereFace = CHMHelpers::GetFace(position);

    // Map spherecoords
    cvar texcoord = CHMHelpers::GetTexcoord(sphereFace, position);

    // Sample CHM
    cvar voxel = m_bitmap->Sample(static_cast<CHMFace::_enum>(sphereFace), texcoord.x, texcoord.y, mipLevel) / 255.0f;

    // the terrain height (over planet, sphere is the base)
    cvar terrainHeight = radius + voxel * height;

    // the height over sphere
    cvar sphereHeight = (position - origin).Length();

    // calculate voxel value
    cvar voxelValue = (sphereHeight - terrainHeight) / lodSize;

    // convert voxel value to voxel
    return VOXEL_FROM_FLOAT(voxelValue);
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

bool VoxelGenerator::GenerateChunkData(const RefPtr<VoxelChunkData>& chunk, const Vector3& position, int lod, int depth)
{
    const int voxelDataSize = VoxelChunkData::ChunkDataSize * VoxelChunkData::ChunkDataSize * VoxelChunkData::ChunkDataSize;
    sbyte voxels[voxelDataSize]; // Well, stack will cry, but game will be still happy

    cvar octree = spaceObject->GetOctree();

    cvar dataSize = VoxelChunkData::ChunkDataSize;
    cvar lod_f = static_cast<float>(lod);
    cvar chunkPosition = spaceObject->GetPosition();

    cvar mipLevel = Math::Clamp(octree->GetMaxDepth() - depth, 0, m_bitmap->GetLoDCount());

    // calculate current voxel size
    cvar lodSize = LoDTable[lod];

    var hasSurface = false;
    sbyte lastVoxel = 0;

    var voxelVolumeSign = 0;
    for (var x = 0; x < dataSize; x++)
    {
        for (var y = 0; y < dataSize; y++)
        {
            for (var z = 0; z < dataSize; z++)
            {
                cvar index = INDEX_3D(x, y, z, dataSize);

                cvar offset = Vector3(float(x), float(y), float(z));
                cvar voxelPosition = position + offset * lod_f;

                cvar value = GenerateFromCHM(chunkPosition, voxelPosition, mipLevel, lodSize,
                    settings->minSurfaceHeight,
                    settings->hillsHeight);

                // Surface checking
                if(!hasSurface && lastVoxel != value && index > 1)
                    hasSurface = true;

                lastVoxel = value;
                voxelVolumeSign += value;
                voxels[index] = value;
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
        memcpy_s(chunkData, voxelDataSize, voxels, voxelDataSize);
    }

    return hasSurface;
}
