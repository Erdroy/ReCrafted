// ReCrafted (c) 2016-2018 Always Too Late

#name "GBuffer Combine"
#desc "GBuffer combine shader"

#define USE_GBUFFERSAMPLING
#include "../ShaderAPI.hlsli"

[Target(CombinePSMain)]
cbuffer Data : register(b0)
{
    float3 lightDir;
};

float3 CalculateLighting(float3 normal) 
{
    float3 dir = normalize(lightDir);
    float nDotL = dot(normal, dir);
    float3 lightColor = float3(1.0f, 0.95f, 0.9f);

    return lightColor * nDotL + float3(0.35f, 0.35f, 0.35f);
}

/// <summary>
/// Pixel Shader Function
/// CombinePSMain
/// </summary>
float4 CombinePSMain(in QuadPS i) : SV_Target0
{
    GBuffer gbuffer = SampleGBuffer(i.UV);
    float3 color = gbuffer.Color;

    color *= CalculateLighting(gbuffer.Normal);

    return float4(color, 1.0f);
}

pass Default
{
    SetProfile(5.0);
    SetVertexShader(QuadVSMain);
    SetPixelShader(CombinePSMain);
}
