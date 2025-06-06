// VectorMath (c) 2018-2019 Damian 'Erdroy' Korczowski

#pragma once

#include "VectorBase.h"
#include "MatrixBase.h"

struct Quaternion : VectorBase<float, 4>
{
protected:
    /* Protected usings */
    using VectorComponents<float, 4>::components;
#if USE_UPPERCASE_COMPONENTS
    using VectorComponents<float, 4>::x;
    using VectorComponents<float, 4>::y;
    using VectorComponents<float, 4>::z;
    using VectorComponents<float, 4>::w;
#else
    using VectorComponents<float, 4>::X;
    using VectorComponents<float, 4>::Y;
    using VectorComponents<float, 4>::Z;
    using VectorComponents<float, 4>::W;
#endif

public:
    /* Public usings */
#if USE_UPPERCASE_COMPONENTS
    using VectorComponents<float, 4>::X;
    using VectorComponents<float, 4>::Y;
    using VectorComponents<float, 4>::Z;
    using VectorComponents<float, 4>::W;
#else
    using VectorComponents<float, 4>::x;
    using VectorComponents<float, 4>::y;
    using VectorComponents<float, 4>::z;
    using VectorComponents<float, 4>::w;
#endif

public:
    /* Constructors */
    Quaternion() = default;

    Quaternion(float x, float y, float z, float w)
    {
        components[0] = x;
        components[1] = y;
        components[2] = z;
        components[3] = w;
    }

public:
    /* Public members */
    void Conjugate();
    void Normalize();
    void Invert();
    void Negate();

    float Length() const;
    float LengthSquared() const;
    float Angle() const;

    VectorBase<float, 3> Axis() const;
    Quaternion Normalized() const;

    bool IsIdentitiy() const;
    bool IsNormalized() const;
    bool IsNaN() const;
    bool IsInfinity() const;

public:
    /* Public static members */
    static Quaternion Conjugate(const Quaternion& q);
    static Quaternion Normalize(const Quaternion& q);
    static Quaternion Invert(const Quaternion& q);
    static Quaternion Negate(const Quaternion& q);

    static float Length(const Quaternion& q);
    static float LengthSquared(const Quaternion& q);
    static float Angle(const Quaternion& q);

    static float Dot(const Quaternion& a, const Quaternion& b);

    static Quaternion Lerp(const Quaternion& from, const Quaternion& to, float amount);
    static Quaternion Slerp(const Quaternion& from, const Quaternion& to, float amount);

    static VectorBase<float, 3> Axis(const Quaternion& q);

    static Quaternion Rotation(const VectorBase<float, 3>& axis, float angle);
    static Quaternion Rotation(float yaw, float pitch, float roll);
    static Quaternion Rotation(const MatrixBase<float, 4, 4>& matrix);

    static bool NearEqual(const Quaternion& a, const Quaternion& b);

    static bool IsNormalized(const Quaternion& q);
    static bool IsIdentitiy(const Quaternion& q);
    static bool IsZero(const Quaternion& q);
    static bool IsNaN(const Quaternion& a);
    static bool IsInfinity(const Quaternion& a);

public:
    /* Operators */
    void operator*=(const Quaternion& other);
    Quaternion operator*(const Quaternion& other) const;
    Quaternion operator*(float other) const;

public:
    /* Constant static members */
    static const Quaternion Identity;
    static const Quaternion Zero;
    static const Quaternion One;
};

inline void Quaternion::Conjugate()
{
    x = -x;
    y = -y;
    z = -z;
}

inline void Quaternion::Normalize()
{
    const auto  lenSqr = LengthSquared();

    if (Math::IsZero(lenSqr))
        return;

    const auto  invLength = 1.0f  / Math::Sqrt(lenSqr);

    x *= invLength;
    y *= invLength;
    z *= invLength;
    w *= invLength;
}

inline void Quaternion::Invert()
{
    const auto invLengthSquared = 1.0f / LengthSquared();

    x = -x * invLengthSquared;
    y = -y * invLengthSquared;
    z = -z * invLengthSquared;
    w = w * invLengthSquared;
}

inline void Quaternion::Negate()
{
    x = -x;
    y = -y;
    z = -z;
    w = -w;
}

inline float Quaternion::Length() const
{
    if (Math::IsZero(LengthSquared()))
        return 0.0f;

    return Math::Sqrt(LengthSquared());
}

inline float Quaternion::LengthSquared() const
{
    return (x * x) + (y * y) + (z * z) + (w * w);
}

inline float Quaternion::Angle() const
{
    if (Math::IsZero(LengthSquared()))
        return 0.0f;

    return float(2.0f * Math::Acos(Math::Clamp(w, -1.0f, 1.0f)));
}

inline VectorBase<float, 3> Quaternion::Axis() const
{
    VectorBase<float, 3> result;
    if (Math::IsZero(LengthSquared()))
    {
        result.x = 1.0f;
        result.y = 0.0f;
        result.z = 0.0f;
        return result;
    }

    const auto invLength = 1.0f / Length();

    result.x = x * invLength;
    result.y = y * invLength;
    result.z = z * invLength;

    return result;
}

inline Quaternion Quaternion::Normalized() const
{
    Quaternion result = *this;
    result.Normalize();
    return result;
}

inline bool Quaternion::IsIdentitiy() const
{
    return IsIdentitiy(*this);
}

inline bool Quaternion::IsNormalized() const
{
    return IsNormalized(*this);
}

inline bool Quaternion::IsNaN() const
{
    return isnan(x) || isnan(y) || isnan(z) || isnan(w);
}

inline bool Quaternion::IsInfinity() const
{
    return isinf(x) || isinf(y) || isinf(z) || isinf(w);
}

inline Quaternion Quaternion::Conjugate(const Quaternion& q)
{
    Quaternion result = q;
    result.Conjugate();
    return result;
}

inline Quaternion Quaternion::Normalize(const Quaternion& q)
{
    Quaternion result = q;
    result.Normalize();
    return result;
}

inline Quaternion Quaternion::Invert(const Quaternion& q)
{
    Quaternion result = q;
    result.Invert();
    return result;
}

inline Quaternion Quaternion::Negate(const Quaternion& q)
{
    Quaternion result = q;
    result.Negate();
    return result;
}

inline float Quaternion::Length(const Quaternion& q)
{
    return q.Length();
}

inline float Quaternion::LengthSquared(const Quaternion& q)
{
    return q.LengthSquared();
}

inline float Quaternion::Angle(const Quaternion& q)
{
    return q.Angle();
}

inline float Quaternion::Dot(const Quaternion& a, const Quaternion& b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

inline Quaternion Quaternion::Lerp(const Quaternion& from, const Quaternion& to, const float amount)
{
    const auto inverse = 1.0f - amount;

    if (Dot(from, to) >= 0.0f)
    {
        return Quaternion(
            (inverse * from.x) + (amount * to.x), 
            (inverse * from.y) + (amount * to.y),
            (inverse * from.z) + (amount * to.z),
            (inverse * from.w) + (amount * to.w)).Normalized();
    }
    
    return Quaternion(
        (inverse * from.x) - (amount * to.x),
        (inverse * from.y) - (amount * to.y),
        (inverse * from.z) - (amount * to.z),
        (inverse * from.w) - (amount * to.w)).Normalized();
}

inline Quaternion Quaternion::Slerp(const Quaternion& from, const Quaternion& to, const float amount)
{
    const auto dot = Dot(from, to);

    float opposite;
    float inverse;
    if (Math::Abs(dot) > 1.0f - Math::ZeroTolerance)
    {
        inverse = 1.0f - amount;
        opposite = amount * Math::Sign(dot);
    }
    else
    {
        const auto acos = Math::Acos(Math::Abs(dot));
        const auto invSin = (1.0f / Math::Sin(acos));

        inverse = Math::Sin((1.0f - amount) * acos) * invSin;
        opposite = Math::Sin(amount * acos) * invSin * Math::Sign(dot);
    }

    return Quaternion(
        (inverse * from.x) - (opposite * to.x),
        (inverse * from.y) - (opposite * to.y),
        (inverse * from.z) - (opposite * to.z),
        (inverse * from.w) - (opposite * to.w)).Normalized();
}

inline VectorBase<float, 3> Quaternion::Axis(const Quaternion& q)
{
    return q.Axis();
}

inline Quaternion Quaternion::Rotation(const VectorBase<float, 3>& axis, const float angle)
{
    const auto invAxisLength = 1.0f / Math::Sqrt((axis.x * axis.x) + (axis.y * axis.y) + (axis.z * axis.z));

    auto normalized = axis;
    normalized.x *= invAxisLength;
    normalized.y *= invAxisLength;
    normalized.z *= invAxisLength;

    const auto half = angle * 0.5f;
    const auto sin = Math::Sin(half);
    const auto cos = Math::Cos(half);

    return Quaternion{ normalized.x * sin, normalized.y * sin, normalized.z * sin, cos };
}

inline Quaternion Quaternion::Rotation(const float yaw, const float pitch, const float roll)
{
    const auto halfRoll = roll * 0.5f;
    const auto halfPitch = pitch * 0.5f;
    const auto halfYaw = yaw * 0.5f;

    const auto sinRoll = Math::Sin(halfRoll);
    const auto cosRoll = Math::Cos(halfRoll);
    const auto sinPitch = Math::Sin(halfPitch);
    const auto cosPitch = Math::Cos(halfPitch);
    const auto sinYaw = Math::Sin(halfYaw);
    const auto cosYaw = Math::Cos(halfYaw);

    return Quaternion{
        (cosYaw * sinPitch * cosRoll) + (sinYaw * cosPitch * sinRoll),
        (sinYaw * cosPitch * cosRoll) - (cosYaw * sinPitch * sinRoll),
        (cosYaw * cosPitch * sinRoll) - (sinYaw * sinPitch * cosRoll),
        (cosYaw * cosPitch * cosRoll) + (sinYaw * sinPitch * sinRoll)
    };
}

inline Quaternion Quaternion::Rotation(const MatrixBase<float, 4, 4>& matrix)
{
    float sqrt;
    float half;

    const auto scale = matrix.m11 + matrix.m22 + matrix.m33;
    auto result = Identity;

    if (scale > 0.0f)
    {
        sqrt = Math::Sqrt(scale + 1.0f);
        result.W = sqrt * 0.5f;
        sqrt = 0.5f / sqrt;

        result.x = (matrix.m23 - matrix.m32) * sqrt;
        result.y = (matrix.m31 - matrix.m13) * sqrt;
        result.z = (matrix.m12 - matrix.m21) * sqrt;
    }
    else if ((matrix.m11 >= matrix.m22) && (matrix.m11 >= matrix.m33))
    {
        sqrt = Math::Sqrt(1.0f + matrix.m11 - matrix.m22 - matrix.m33);
        half = 0.5f / sqrt;

        result.x = 0.5f * sqrt;
        result.y = (matrix.m12 + matrix.m21) * half;
        result.z = (matrix.m13 + matrix.m31) * half;
        result.W = (matrix.m23 - matrix.m32) * half;
    }
    else if (matrix.m22 > matrix.m33)
    {
        sqrt = Math::Sqrt(1.0f + matrix.m22 - matrix.m11 - matrix.m33);
        half = 0.5f / sqrt;

        result.x = (matrix.m21 + matrix.m12) * half;
        result.y = 0.5f * sqrt;
        result.z = (matrix.m32 + matrix.m23) * half;
        result.W = (matrix.m31 - matrix.m13) * half;
    }
    else
    {
        sqrt = Math::Sqrt(1.0f + matrix.m33 - matrix.m11 - matrix.m22);
        half = 0.5f / sqrt;

        result.x = (matrix.m31 + matrix.m13) * half;
        result.y = (matrix.m32 + matrix.m23) * half;
        result.z = 0.5f * sqrt;
        result.W = (matrix.m12 - matrix.m21) * half;
    }

    return result;
}

inline bool Quaternion::NearEqual(const Quaternion& a, const Quaternion& b)
{
    return Math::NearEqual(a.x, b.x) && Math::NearEqual(a.y, b.y) && Math::NearEqual(a.z, b.z) && Math::NearEqual(a.w, b.w);
}

inline bool Quaternion::IsNormalized(const Quaternion& q)
{
    return Math::IsOne(q.LengthSquared());
}

inline bool Quaternion::IsIdentitiy(const Quaternion& q)
{
    return q.x == 0 && q.y == 0 && q.z == 0 && q.w == 1.0f;
}

inline bool Quaternion::IsZero(const Quaternion& q)
{
    return q.x == 0 && q.y == 0 && q.z == 0 && q.w == 0;
}

inline bool Quaternion::IsNaN(const Quaternion& a)
{
    return a.IsNaN();
}

inline bool Quaternion::IsInfinity(const Quaternion& a)
{
    return a.IsInfinity();
}

inline void Quaternion::operator*=(const Quaternion& other)
{
    const auto lx = X;
    const auto ly = Y;
    const auto lz = Z;
    const auto lw = W;
    const auto rx = other.X;
    const auto ry = other.Y;
    const auto rz = other.Z;
    const auto rw = other.W;
    const auto a = (ly * rz - lz * ry);
    const auto b = (lz * rx - lx * rz);
    const auto c = (lx * ry - ly * rx);
    const auto d = (lx * rx + ly * ry + lz * rz);

    x = (lx * rw + rx * lw) + a;
    y = (ly * rw + ry * lw) + b;
    z = (lz * rw + rz * lw) + c;
    w = lw * rw - d;
}

inline Quaternion Quaternion::operator*(const Quaternion& other) const
{
    auto result = *this;
    result *= other;
    return result;
}

inline Quaternion Quaternion::operator*(const float other) const
{
    auto result = *this;
    result.x *= other;
    result.y *= other;
    result.z *= other;
    result.w *= other;
    return result;
}
