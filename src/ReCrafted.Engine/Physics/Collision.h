// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef COLLISION_H
#define COLLISION_H

#include "ReCrafted.h"
#include "Physics/IPhysicsShapeCooker.h"
#include "Scripting/Object.h"

class Collision : public Object
{
    friend class Object;
    friend class RigidBodyActor;

    enum Type
    {
        Unknown,

        Box,
        Sphere,
        Capsule,
        TriangleMesh,
        ConvexHullMesh,

        Count
    };

private:
    SCRIPTING_API_IMPL()

private:
    IPhysicsShape* m_shape = nullptr;
    Type m_type = Unknown;

    bool m_attached = false;

private:
    Collision() = default;

private:
    const char* GetObjectName() const override
    {
        return "Collision";
    }

public:
    void OnDestroy() override;

private:
    static Collision* CreateInstance();

public:
    static Collision* CreateBoxCollision(const Vector3& extents);
    static Collision* CreateSphereCollision(float radius);
    static Collision* CreateCapsuleCollision(float radius, float halfHeight);

public:
    static Collision* CreateTriangleMeshCollision(Vector3* points, size_t numPoints, uint32_t* indices,
                                                  size_t numIndices, IPhysicsShapeCooker* shapeCooker = nullptr);
    static Collision* CreateTriangleMeshCollision(void* triangleMeshShape, IPhysicsShapeCooker* shapeCooker = nullptr);

    static Collision* CreateConvexHullMeshCollision(Vector3* points, size_t numPoints, uint32_t* indices,
                                                    size_t numIndices, IPhysicsShapeCooker* shapeCooker = nullptr);
    static Collision* CreateConvexHullMeshCollision(void* convexHullMeshShape, IPhysicsShapeCooker* shapeCooker = nullptr);
};

#endif // COLLISION_H
