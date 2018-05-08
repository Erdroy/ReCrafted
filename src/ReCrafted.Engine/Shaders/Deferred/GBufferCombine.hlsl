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

/// <summary>
/// Pixel Shader Function
/// CombinePSMain
/// </summary>
float4 CombinePSMain(in QuadPS i) : SV_Target0
{
    GBuffer gbuffer = SampleGBuffer(i.UV);

    return float4(gbuffer.Color, 1.0f);
}

pass Default
{
    SetProfile(5.0);
    SetVertexShader(QuadVSMain);
    SetPixelShader(CombinePSMain);
}
