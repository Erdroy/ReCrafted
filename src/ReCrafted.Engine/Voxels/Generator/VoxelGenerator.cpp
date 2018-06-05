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

bool VoxelGenerator::GenerateChunkData(sbyte* voxelData, const Vector3& position, int lod)
{
    cvar dataSize = VoxelChunkData::ChunkDataSize;
    cvar lod_f = static_cast<float>(lod);
    cvar chunkPosition = spaceObject->GetPosition();

    // calculate current voxel size
    cvar lodSize = LoDTable[lod];

    for (var x = 0; x < dataSize; x++)
    {
        for (var y = 0; y < dataSize; y++)
        {
            for (var z = 0; z < dataSize; z++)
            {
                cvar index = INDEX_3D(x, y, z, dataSize);

                cvar offset = Vector3(float(x), float(y), float(z));
                cvar voxelPosition = position + offset * lod_f;

                cvar value = GenerateFromCHM(chunkPosition, voxelPosition, 0, lodSize,
                    settings->minSurfaceHeight,
                    settings->hillsHeight);

                voxelData[index] = value;
            }
        }
    }

    // TODO: Check if chunk has surface
    return true;
}
