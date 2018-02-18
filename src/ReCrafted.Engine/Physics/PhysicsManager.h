// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

// includes
#include "Core/IDisposable.h"

class PhysicsManager : IDisposable
{
private:
public:
    void init();
    void update();
    void dispose() override;
};

#endif // PHYSICSMANAGER_H
