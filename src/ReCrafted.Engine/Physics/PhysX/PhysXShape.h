// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PHYSXSHAPE_H
#define PHYSXSHAPE_H

// includes
#include "ReCrafted.h"
#include "Core/Math/Math.h"
#include "Physics/IPhysicsShape.h"

#include "PhysX.h"

class PhysXShape : public IPhysicsShape
{
public:
    PxShape* shape = nullptr;

public:
    explicit PhysXShape(PxShape* shape) : shape(shape) {}
};

#endif // PHYSXSHAPE_H
