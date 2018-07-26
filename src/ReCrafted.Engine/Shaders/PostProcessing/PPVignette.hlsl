// ReCrafted (c) 2016-2018 Always Too Late

#name "PPVignette"
#desc "Vignette post processing shader"

#define PRESET_POSTPROCESSING
#include "../ShaderAPI.hlsli"

float4 PostProcessMain(in float4 color, in float2 uv, in float depth)
{
    float alpha = color.a;

    // Apply Vignetting
    float2 distance = uv - 0.5f;
    distance.x = 0.5f - dot(distance, distance);
    color *= pow(abs(distance.x), 0.2f);

    return float4(color.rgb, alpha);
}