// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef IPHYSICSENGINE_H
#define IPHYSICSENGINE_H

// includes
#include "ReCrafted.h"
#include "Common/Transform.h"
#include "Physics/IPhysicsScene.h"
#include "Physics/IPhysicsShapeCooker.h"

class IPhysicsShape;
class IPhysicsCharacter;
class IPhysicsActor;
struct TransformComponent;
struct PhysicsBodyComponent;
struct PhysicsShapeComponent;

class IPhysicsEngine
{
public:
    virtual ~IPhysicsEngine() = default;

public:
    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Simulate() = 0;
    virtual void Shutdown() = 0;

public:
    virtual IPhysicsShapeCooker* CreateCooker() = 0;
    virtual IPhysicsShapeCooker* GetDefaultCooker() = 0;
    virtual void ReleaseCooker(IPhysicsShapeCooker* cooker) = 0;

    virtual IPhysicsActor* CreateActor(const Transform& transform, bool dynamic) = 0;
    virtual void ReleaseActor(IPhysicsActor* actor) = 0;

    virtual IPhysicsShape* CreateBoxShape(const Vector3& extents) = 0;
    virtual IPhysicsShape* CreateSphereShape(float radius) = 0;
    virtual IPhysicsShape* CreateCapsuleShape(float radius, float halfHeight) = 0;
    virtual IPhysicsShape* CreateTriangleMeshShape(void* shapePtr) = 0;
    virtual IPhysicsShape* CreateConvexHullMeshShape(void* shapePtr) = 0;
    virtual void ReleaseShape(IPhysicsShape* shape) = 0;

    virtual RefPtr<IPhysicsScene> CreateScene() = 0;
    virtual void DestroyScene(RefPtr<IPhysicsScene>& scene) = 0;
};

#endif // IPHYSICSENGINE_H
