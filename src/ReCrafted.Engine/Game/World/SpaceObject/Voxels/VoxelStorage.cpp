// ReCrafted © 2016-2017 Always Too Late

#include "VoxelStorage.h"
#include "VoxelUtils.h"
#include "../SpaceObjectSettings.h"
#include "Core/Math/Math.h"

float Planet(const Vector3& origin, const Vector3& position, float radius)
{
    //var local = position - Vector3(radius, radius, radius);
    //var d = Vector3(fabs(local.x), fabs(local.y), fabs(local.z)) - Vector3(radius, radius, radius);
    //var m = max(d.x, max(d.y, d.z)); 
    //return min(m, d.length());

    var height = (position - origin).length();
    return height - radius;
}


void VoxelStorage::init(SpaceObjectSettings& settings)
{

}

sbyte VoxelStorage::getVoxel(Vector3 position)
{
    return VOXEL_FROM_FLOAT(Planet(spaceObject->m_position, position, spaceObject->m_settings.minSurfaceHeight)); // convert to voxel data
}
