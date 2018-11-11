// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef TRANSFORM_H
#define TRANSFORM_H

// includes
#include "ReCrafted.h"

struct Transform
{
public:
    Vector3 translation = Vector3::Zero;
    Quaternion orientation = Quaternion::Identity;
    Vector3 scale = Vector3::One;

public:
    Transform() = default;
    Transform(const Vector3 translation, const Quaternion orientation, const Vector3 scale) :
        translation(translation)
        , orientation(orientation)
        , scale(scale) {}

public:
    Vector3 TransformPoint(const Vector3& point) const
    {
        return Vector3::Transform(point, orientation);
    }

public:
    Matrix GetTransformationMatrix() const
    {
        return Matrix::CreateTransform(translation, orientation, scale);
    }

public: /* Space transformations */
    Transform ToWorld(const Transform& transform) const
    {
        Transform result;

        // Translation
        result.translation = ToWorld(transform.translation);

        // Orientation
        result.orientation = ToWorld(transform.orientation);

        // Scale
        result.scale = scale * transform.scale;

        return result;
    }

    Vector3 ToWorld(const Vector3& point) const
    {
        return TransformPoint(point * scale) + translation;
    }

    Quaternion ToWorld(const Quaternion& rotation) const
    {
        return Quaternion::Normalize(orientation * rotation);
    }

    Transform ToLocal(Transform& transform) const
    {
        Transform result;

        // Translation
        result.translation = ToLocal(transform.translation);

        // Orientation
        result.orientation = ToLocal(transform.orientation);

        // Scale
        result.scale = transform.scale * (Vector3::One / scale);

        return result;
    }

    Vector3 ToLocal(const Vector3& point) const
    {
        return Vector3::Transform(point - translation, Quaternion::Invert(orientation)) * (Vector3::One / scale);
    }

    Quaternion ToLocal(const Quaternion& rotation) const
    {
        return Quaternion::Normalize(Quaternion::Invert(orientation) * rotation);
    }

public:
    static const Transform Identity;
};



#endif // TRANSFORM_H
