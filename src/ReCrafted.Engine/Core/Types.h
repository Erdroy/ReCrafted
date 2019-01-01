// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef TYPES_H
#define TYPES_H

// includes
#include <memory>

#include "Core/Math/Vector2Base.h"
#include "Core/Math/Vector3Base.h"
#include "Core/Math/Vector4Base.h"
#include "Core/Math/Quaternion.h"
#include "Core/Math/Matrix4x4Base.h"


typedef unsigned char byte;

typedef signed char sbyte;

typedef unsigned int uint;

template <class T>
using RefPtr = std::shared_ptr<T>;

template <class T>
using Ref = std::reference_wrapper<T>;


using Vector2f = Vector2Base<float>;
using Vector3f = Vector3Base<float>;
using Vector4f = Vector4Base<float>;

using Vector2d = Vector2Base<double>;
using Vector3d = Vector3Base<double>;
using Vector4d = Vector4Base<double>;

using Matrix4x4f = Matrix4x4Base<float>;
using Matrix4x4d = Matrix4x4Base<double>;

using Matrix = Matrix4x4f; // Maybe we want to also make the matrix double precision?

using Vector2 = Vector2f;
using Vector3 = Vector3f;
using Vector4 = Vector4f;

#endif // TYPES_H
