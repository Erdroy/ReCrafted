// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef IPHYSICSENGINE_H
#define IPHYSICSENGINE_H

// includes
#include "ReCrafted.h"
#include "IPhysicsScene.h"
#include "IPhysicsShapeCooker.h"

class IPhysicsShape;
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
    virtual void ReleaseCooker(IPhysicsShapeCooker* cooker) = 0;

    virtual IPhysicsActor* CreateActor(const TransformComponent& transform, PhysicsBodyComponent& body) = 0;
    virtual void ReleaseActor(IPhysicsActor* actor) = 0;

    virtual IPhysicsShape* CreateShape(const TransformComponent& transform, const PhysicsShapeComponent& shape) = 0;
    virtual void ReleaseShape(IPhysicsShape* shape) = 0;

    virtual RefPtr<IPhysicsScene> CreateScene() = 0;
    virtual void DestroyScene(RefPtr<IPhysicsScene>& scene) = 0;
};

#endif // IPHYSICSENGINE_H
