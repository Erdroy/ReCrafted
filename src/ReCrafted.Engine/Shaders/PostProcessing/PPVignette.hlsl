// ReCrafted (c) 2016-2018 Always Too Late

#name "PPVignette"
#desc "Vignette post processing shader"

#define PRESET_POSTPROCESSING
#include "../ShaderAPI.hlsli"

float4 PostProcessMain(in float4 color, in float depth)
{
    return float4(1.0f, 0.0f, 0.0f, 1.0f);
}