// ReCrafted © 2016-2017 Always Too Late

#include "VoxelStorage.h"
#include "VoxelUtils.h"
#include "Core/Math/Math.h"
#include "Core/Logger.h"
#include "Game/World/SpaceObject/SpaceObjectChunk.h"
#include "Game/World/SpaceObject/SpaceObjectSettings.h"

float Planet(const Vector3& origin, const Vector3& position, const float radius)
{
    cvar height = (position - origin).length();
    return height - radius;
}

sbyte* VoxelStorage::generateChunkFromCHM(const Vector3 position, const int lod)
{
    cvar dataSize = SpaceObjectChunk::ChunkDataSize;
    cvar lod_f = static_cast<float>(lod);
    cvar data = new sbyte[dataSize * dataSize * dataSize];
    
    // TODO: map texture coord
    // TODO: sample height from texture (using bicubic filtering)
    // TODO: generate voxel using sphere SDF and sampled height
    // TODO: return nullptr where there is no any proper voxel surface (the chunk is completely above or under surface)

    // tempoarary
    for (var x = 0; x < dataSize; x++)
    {
        for (var y = 0; y < dataSize; y++)
        {
            for (var z = 0; z < dataSize; z++)
            {
                cvar index = INDEX_3D(x, y, z, dataSize);

                cvar offset = Vector3(float(x), float(y), float(z));
                cvar voxelPosition = position + offset * lod_f;

                data[index] = getVoxel(voxelPosition);
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
    else
    {
        // TODO: load chm sides (VoxelCHM class neded) (up, down, left, right, front and back) [*.png files]
    }
}

void VoxelStorage::dispose()
{
    // release all resources
}

sbyte VoxelStorage::getVoxel(Vector3 position)
{
    return VOXEL_FROM_FLOAT(Planet(spaceObject->m_position, position, spaceObject->m_settings.minSurfaceHeight)); // convert to voxel data
}

sbyte* VoxelStorage::getVoxelChunk(const Vector3 position, const int lod)
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
