// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "Common/Entities/Component.h"

struct TransformComponent : Component
{
    Vector3 position;
    Quaternion rotation;
};

#endif // TRANSFORMCOMPONENT_H
