// ReCrafted © 2016-2017 Always Too Late

#include "VoxelStorage.h"
#include "VoxelUtils.h"
#include "VoxelCHM.h"
#include "Core/Math/Math.h"
#include "Core/Logger.h"
#include "Game/World/SpaceObject/LODTable.h"
#include "Game/World/SpaceObject/SpaceObjectChunk.h"
#include "Game/World/SpaceObject/SpaceObjectSettings.h"

FORCEINLINE sbyte sdf_planet_generate(VoxelCHM* chm, const Vector3& origin, const Vector3& position, const int lod, const float radius, const float hillsHeight)
{
    // calculate current voxel size
    cvar lodSize = lodtable[lod];

    // the terrain height (over planet, sphere is the base)
    cvar terrainHeight = radius + chm->sample(position, radius, lod) * hillsHeight;

    // the height over sphere
    cvar sphereHeight = (position - origin).length();

    // calculate voxel value
    cvar voxelValue = (sphereHeight - terrainHeight) / lodSize;

    // convert voxel value to voxel
    return VOXEL_FROM_FLOAT(voxelValue);
}

sbyte* VoxelStorage::generateChunkFromCHM(Vector3& position, const int lod)
{
    cvar chm = m_chm.get();
    cvar dataSize = SpaceObjectChunk::ChunkDataSize;
    cvar lod_f = static_cast<float>(lod);
    cvar data = new sbyte[dataSize * dataSize * dataSize];

    // TODO: apply modifications applyCSG or something
    // TODO: return nullptr where there is no any proper voxel surface (the chunk is completely above or under surface)

    for (var x = 0; x < dataSize; x++)
    {
        for (var y = 0; y < dataSize; y++)
        {
            for (var z = 0; z < dataSize; z++)
            {
                cvar index = INDEX_3D(x, y, z, dataSize);

                cvar offset = Vector3(float(x), float(y), float(z));
                cvar voxelPosition = position + offset * lod_f;

                // tempoarary
                data[index] = sdf_planet_generate(chm, spaceObject->m_position, voxelPosition, lod, spaceObject->m_settings.minSurfaceHeight, spaceObject->m_settings.hillsHeight);
            }
        }
    }

    return data;
}

void VoxelStorage::init(SpaceObjectSettings& settings)
{
    this->settings = &settings;

    if(settings.generationType == GenerationType::PreGenerated)
    {
        // TODO: load pregen (VoxelPregen class neded) [*.rcv file]
        Logger::logError("getVoxelsRow for pregens is not implemented!");
        return;
    }

    m_chm = VoxelCHM::loadFromDirectory(settings.fileName);
}

void VoxelStorage::dispose()
{
    // release all resources
}

sbyte* VoxelStorage::getVoxelChunk(Vector3& position, const int lod)
{
    if (settings->generationType == GenerationType::PreGenerated)
    {
        // TODO: pregens
        Logger::logError("getVoxelsRow for pregens is not implemented!");
        return nullptr;
    }

    // generate chunk now using CHM
    return generateChunkFromCHM(position, lod);
}
