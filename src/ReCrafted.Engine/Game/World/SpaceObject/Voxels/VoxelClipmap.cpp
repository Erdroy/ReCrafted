// ReCrafted © 2016-2017 Always Too Late

#include "VoxelClipmap.h"
#include "Game/World/SpaceObject/SpaceObjectSettings.h"

void VoxelClipmap::init(SpaceObjectSettings& settings)
{
}

void VoxelClipmap::dispose()
{
}

bool VoxelClipmap::chunkModified(const Vector3& position, int lod)
{
    return false;
}

void VoxelClipmap::chunkModify(const Vector3& position, int lod, sbyte* voxels)
{

}
