// ReCrafted (c) 2016-2018 Always Too Late

#ifndef LIGHTING_HLSLI
#define LIGHTING_HLSLI

float3 CalculateLightingSimple(float3 surfaceNormal, float3 lightDirection, float3 ambientLightColor, float3 directionalLightColor, float lightPower)
{
    float3 NoL = dot(surfaceNormal, lightDirection);
    return directionalLightColor * NoL * lightPower + ambientLightColor;
}

#endif // LIGHTING_HLSLI
