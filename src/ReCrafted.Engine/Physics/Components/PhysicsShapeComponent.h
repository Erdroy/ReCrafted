// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PHYSICSSHAPECOMPONENT_H
#define PHYSICSSHAPECOMPONENT_H

#include "Common/Entities/Component.h"
#include "Physics/IPhysicsEngine.h"
#include "Physics/IPhysicsShape.h"

struct PhysicsShapeComponent : Component
{
    enum Type : uint8_t
    {
        Box,
        Sphere,
        TriangleMesh
    };

public:
    PhysicsShapeComponent() = default;
    explicit PhysicsShapeComponent(const Type type, const Vector3 extents = Vector3::One(), const float radius = 1.0f) :
        type(type), extents(extents), radius(radius)
    {
        
    }

public:
    IPhysicsShape* physicsShape = nullptr;

public:
    Type type = Box;
    Vector3 extents = Vector3::One();
    float radius = 1.0f;
};

#endif // PHYSICSSHAPECOMPONENT_H
