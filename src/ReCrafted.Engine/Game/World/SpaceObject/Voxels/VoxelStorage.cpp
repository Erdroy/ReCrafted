// ReCrafted © 2016-2017 Always Too Late

#include "VoxelStorage.h"
#include "VoxelUtils.h"
#include "VoxelCHM.h"
#include "Core/Math/Math.h"
#include "Core/Logger.h"
#include "Game/World/SpaceObject/SpaceObjectChunk.h"
#include "Game/World/SpaceObject/SpaceObjectSettings.h"

FORCEINLINE sbyte generateFromCHM(VoxelCHM* chm, Vector3& origin, const Vector3& position, const int lod, const float radius, const float hillsHeight)
{
    cvar localHeight = radius + (chm->sample(position, radius, lod)) * hillsHeight;
    cvar height = (position - origin).length();
    return VOXEL_FROM_FLOAT(radius + (height - localHeight));
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
                data[index] = generateFromCHM(chm, spaceObject->m_position, voxelPosition, lod, spaceObject->m_settings.minSurfaceHeight, spaceObject->m_settings.hillsHeight);
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

    // TODO: VoxelClipmap class (to store modifications, should allow binary (de) serialization (streams needed! :D))
    // TODO: VoxelChunkCache class (should store chunks for some time or all the time if chunk is still being used)

    // TODO: check if this is cached (use start and lod) and return if present
    // TODO: if not, generate and apply modifications if present

    // generate chunk now using CHM
    return generateChunkFromCHM(position, lod);
}
