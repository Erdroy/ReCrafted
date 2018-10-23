// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PHYSICSBODYCOMPONENT_H
#define PHYSICSBODYCOMPONENT_H

#include "Common/Entities/Component.h"

struct PhysicsBodyComponent : Component
{
    bool dynamic = true;
};

#endif // PHYSICSBODYCOMPONENT_H
