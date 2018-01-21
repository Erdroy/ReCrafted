// ReCrafted (c) 2016-2018 Always Too Late

#include "VoxelClipmap.h"
#include "Core/Math/Math.h"
#include "Game/World/SpaceObject/SpaceObjectSettings.h"
#include "Game/World/SpaceObject/SpaceObjectChunk.h"

void VoxelClipmap::init(SpaceObjectSettings& settings)
{
}

void VoxelClipmap::dispose()
{
}

uint32_t VoxelClipmap::addCSGShape(CSGMode::_enum mode, CSGShape::_enum shape, const Vector3& position, const float size)
{
    // TODO: apply modifications

    return 0u;
}

void VoxelClipmap::removeCSGShape(uint32_t id)
{

}

bool VoxelClipmap::chunkModified(const Vector3& position, int lod)
{
    return true;
}

void VoxelClipmap::chunkModify(const Vector3& position, int lod, sbyte* voxels)
{
    cvar lodf = static_cast<float>(lod);
    cvar dataSize = SpaceObjectChunk::ChunkDataSize;

}
