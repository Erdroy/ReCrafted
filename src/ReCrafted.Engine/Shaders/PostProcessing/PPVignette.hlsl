// ReCrafted (c) 2016-2018 Always Too Late

#name "PPVignette"
#desc "Vignette post processing shader"

#define PRESET_POSTPROCESSING
#include "../ShaderAPI.hlsli"

#define VIGNETTE_RADIUS 1.45f
#define VIGNETTE_SOFTNESS 0.7f
#define VIGNETTE_OPACITY 0.5f

float4 PostProcessMain(in float4 color, in float2 uv, in float depth)
{
    // Apply Vignetting
    float2 position = uv - float2(0.5f, 0.5f);
    float len = length(position) * 2.0f;
    float vignette = smoothstep(VIGNETTE_RADIUS, VIGNETTE_RADIUS - VIGNETTE_SOFTNESS, len);
    color.rgb = lerp(color.rgb, color.rgb * vignette, VIGNETTE_OPACITY);

    return color;
}

pass Default
{
    SetProfile(5.0);
    SetVertexShader(PostProcessingVSMain);
    SetPixelShader(PostProcessingPSMain);
}