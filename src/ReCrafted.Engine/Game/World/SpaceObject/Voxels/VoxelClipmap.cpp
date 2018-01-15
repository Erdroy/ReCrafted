// ReCrafted (c) 2016-2018 Always Too Late

#include "VoxelClipmap.h"
#include "VoxelUtils.h"
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
    var spherePos = Vector3(0, 0, 0);
    
    cvar dataSize = SpaceObjectChunk::ChunkDataSize;

    for (var x = 0; x < dataSize; x++)
    for (var y = 0; y < dataSize; y++)
    for (var z = 0; z < dataSize; z++)
    {
        cvar offset = Vector3(float(x), float(y), float(z)) * lodf;
        var point = position + offset;
        point.y = 0.0f;
        
        if(Vector3::distance(point, spherePos) < 25.0f)
        {
            voxels[INDEX_3D(x, y, z, dataSize)] = VOXEL_FROM_FLOAT(1.0f);
        }
    }

    // TODO: get all CSG modifications for this chunk
    // TODO: apply all CSG modifications:
    //  TODO: get CSG bounds and clip them to the chunk box
    //  TODO: iterate all voxels and check if some of those are inside or outside CSG operations
    //  TODO: apply operation if needed
}
