// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef TRANSFORM_H
#define TRANSFORM_H

// includes
#include "ReCrafted.h"

struct Transform
{
public:
    Transform* m_parent = nullptr;
    Vector3 m_position = Vector3::Zero;
    Quaternion m_rotation = Quaternion::Identity;

public:
    Transform() = default;
    Transform(Transform* parent, const Vector3 position, const Quaternion rotation) :
          m_parent(parent)
        , m_position(position)
        , m_rotation(rotation) {}

public:

public:
    static const Transform Identity;
};



#endif // TRANSFORM_H
