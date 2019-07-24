// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include "Common/Math/Config.h"
#include "Common/Math/Math.h"
#include "Common/Math/Vector2Base.h"
#include "Common/Math/Vector3Base.h"
#include "Common/Math/Vector4Base.h"
#include "Common/Math/Quaternion.h"
#include "Common/Math/Matrix4x4Base.h"
#include "Common/Math/PlaneBase.h"
#include "Common/Math/BoundingBoxBase.h"
#include "Common/Math/BoundingFrustumBase.h"
#include "Common/Math/ColorBase.h"

using Vector2f = Vector2Base<float>;
using Vector3f = Vector3Base<float>;
using Vector4f = Vector4Base<float>;

using Vector2d = Vector2Base<double>;
using Vector3d = Vector3Base<double>;
using Vector4d = Vector4Base<double>;

using Vector2i = Vector3Base<int32_t>;
using Vector3i = Vector3Base<int32_t>;
using Vector4i = Vector3Base<int32_t>;

using Matrix4x4f = Matrix4x4Base<float>;
using Matrix4x4d = Matrix4x4Base<double>;

using Matrix = Matrix4x4f;
using MatrixD = Matrix4x4d;

using Vector2 = Vector2f;
using Vector3 = Vector3f;
using Vector4 = Vector4f;

using Plane = PlaneBase<float>;
using BoundingFrustum = BoundingFrustumBase<float>;
using BoundingBox = BoundingBoxBase<float>;
using BoundingSphere = BoundingSphereBase<float>;

using PlaneD = PlaneBase<float>;
using BoundingFrustumD = BoundingFrustumBase<float>;
using BoundingBoxD = BoundingBoxBase<double>;
using BoundingSphereD = BoundingSphereBase<double>;

using Color = ColorBase<float>;
using Color4 = Color;

typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned int uint;

template <class T>
using RefPtr = std::shared_ptr<T>;

template <class T>
using Ref = std::reference_wrapper<T>;
