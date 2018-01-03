// ReCrafted (c) 2016-2018 Always Too Late

#include "VoxelClipmap.h"
#include "Game/World/SpaceObject/SpaceObjectSettings.h"

void VoxelClipmap::init(SpaceObjectSettings& settings)
{
}

void VoxelClipmap::dispose()
{
}

uint32_t VoxelClipmap::addCSGShape(CSGMode::_enum mode, CSGShape::_enum shape, const Vector3& position, const float size)
{
    return 0u;
}

void VoxelClipmap::removeCSGShape(uint32_t id)
{

}

bool VoxelClipmap::chunkModified(const Vector3& position, int lod)
{
    return false;
}

void VoxelClipmap::chunkModify(const Vector3& position, int lod, sbyte* voxels)
{

}
