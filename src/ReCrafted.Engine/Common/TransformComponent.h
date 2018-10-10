// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "Common/Entities/Component.h"
#include "Core/Math/Vector3.h"

struct TransformComponent : Component
{
    Vector3 position;
    Vector3 rotation;
};

#endif // TRANSFORMCOMPONENT_H
