// ReCrafted © 2016-2017 Always Too Late

#include "VoxelStorage.h"
#include "VoxelUtils.h"
#include "../SpaceObjectSettings.h"
#include "Core/Math/Math.h"
#include "Core/Logger.h"

float Planet(const Vector3& origin, const Vector3& position, const float radius)
{
    cvar height = (position - origin).length();
    return height - radius;
}

sbyte* VoxelStorage::generateChunkFromCHM(Vector3 position, int lod)
{

    return nullptr;
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

sbyte* VoxelStorage::getVoxelChunk(Vector3 position, int lod)
{
    if (settings->generationType == GenerationType::PreGenerated)
    {
        // TODO: pregens
        Logger::logError("getVoxelsRow for pregens is not implemented!");
        return nullptr;
    }

    // TODO: VoxelClipmap class (to store modifications, should allow binary (de) serialization (streams needed! :D))
    // TODO: VoxelChunkCache class

    // TODO: check if this is cached (use start and lod) and return if present
    // TODO: if not, generate

    // generate chunk now

    return nullptr;
}
