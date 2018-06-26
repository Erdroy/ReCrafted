// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

// includes
#include "Core/EngineComponent.h"

class PhysicsEngine : public EngineComponent<PhysicsEngine>
{
private:
    friend class EngineMain;

public:
    virtual ~PhysicsEngine() = default;

private:
    void OnInit() override;
    void OnDispose() override;
    void Update() override;
    void Simulate();
};

#endif // PHYSICSENGINE_H
