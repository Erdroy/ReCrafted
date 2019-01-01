// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef PHYSX_H
#define PHYSX_H

#include <PxPhysicsAPI.h>
using namespace physx;

#define FromPxV2(vec) Vector2(vec.x, vec.y)
#define FromPxV3(vec) Vector3(vec.x, vec.y, vec.z)
#define FromPxV3Ex(vec) Vector3(float(vec.x), float(vec.y), float(vec.z))
#define FromPxV4(vec) Vector4(vec.x, vec.y, vec.z, vec.w)
#define FromPxQ(vec) Quaternion(vec.x, vec.y, vec.z, vec.w)

#define ToPxV2(vec) PxVec2(vec.x, vec.y)
#define ToPxV3(vec) PxVec3(vec.x, vec.y, vec.z)
#define ToPxV3Ex(vec) PxExtendedVec3(double(vec.x), double(vec.y), double(vec.z))
#define ToPxV4(vec) PxVec4(vec.x, vec.y, vec.z, vec.w)
#define ToPxQ(vec) PxQuat(vec.x, vec.y, vec.z, vec.w)

#endif // PHYSX_H