// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef IPHYSICSENGINE_H
#define IPHYSICSENGINE_H

// includes
#include "ReCrafted.h"
#include "IPhysicsScene.h"

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
    virtual RefPtr<IPhysicsScene> CreateScene() = 0;
    virtual void DestroyScene(RefPtr<IPhysicsScene>& scene) = 0;
};

#endif // IPHYSICSENGINE_H
