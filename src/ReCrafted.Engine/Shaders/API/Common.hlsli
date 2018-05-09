// ReCrafted (c) 2016-2018 Always Too Late

#ifndef COMMON_HLSLI
#define COMMON_HLSLI

#ifdef USE_3DSPACE
cbuffer DefaultContants : register(b0)
{
    matrix MVP;

#ifdef USE_LOGZBUFFER
    float nearPlane;
    float farPlane;
#endif // USE_LOGZBUFFER
}
#endif // USE_3DSPACE

#define TransformPosition3D(pos) mul(MVP, float4(pos, 1.0f));
#define TransformPosition2D(pos) float4(pos, 1.0f);

#define EncodeNormal(n) normalize(n) * 0.5f + 0.5f;

#ifdef USE_3DSPACE
#define TransformPosition(pos) TransformPosition3D(pos)
#else
#define TransformPosition(pos) TransformPosition2D(pos)
#endif // USE_3DSPACE

#endif
