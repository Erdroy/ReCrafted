// ReCrafted (c) 2016-2018 Always Too Late

#ifndef MATH_HLSLI
#define MATH_HLSLI

#define PI 3.1415926535897932

/// <summary>
/// 3x3 Rotation matrix with an angle and an arbitrary vector
/// Source: https://gist.github.com/ee439d5e7388f3aafc5296005c8c3f33
/// </summary>
float3x3 rotateAngleAxis(float angle, float3 axis)
{
    float c, s;
    sincos(angle, s, c);

    float t = 1 - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    return float3x3(
        t * x * x + c, t * x * y - s * z, t * x * z + s * y,
        t * x * y + s * z, t * y * y + c, t * y * z - s * x,
        t * x * z - s * y, t * y * z + s * x, t * z * z + c
        );
}

#endif // MATH_HLSLI