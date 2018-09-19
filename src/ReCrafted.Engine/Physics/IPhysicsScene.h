// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef IPHYSICSSCENE_H
#define IPHYSICSSCENE_H

// includes
#include "ReCrafted.h"

class IPhysicsScene
{
public:
    virtual ~IPhysicsScene() = default;

protected:
    virtual void Update() = 0;
    virtual void Simulate() = 0;
    virtual void Shutdown() = 0;
};

#endif // IPHYSICSSCENE_H
