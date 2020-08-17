// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#ifndef LIGHTING_HLSLI
#define LIGHTING_HLSLI

float3 CalculateLightingSimple(float3 surfaceNormal, float3 lightDirection, float3 directionalLightColor, float lightPower)
{
    float3 NoL = saturate(dot(surfaceNormal, lightDirection));
    return directionalLightColor * lightPower * NoL;
}

#endif // LIGHTING_HLSLI
