// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#name "GBuffer Combine"
#desc "GBuffer combine shader"

#define USE_FULLSCREENQUAD
#define USE_GBUFFERSAMPLING

#include "../ShaderAPI.hlsli"

/// <summary>
/// Pixel Shader Function
/// CombinePSMain
/// </summary>
float4 CombinePSMain(in QuadPS i) : SV_Target0
{
    GBuffer gbuffer = SampleGBuffer(i.UV);

    // Linearize depth
    const float C = 1.0f;
    const float FC = 1.0 / log(FarPlane * C + 1);
    float depth = (exp(gbuffer.Depth * log(FarPlane * C + 1)) - 1) / C;
    depth /= FarPlane; // Normalize into [0-1] range

    // Clip depth
    clip(0.99999f - depth);

    // Calculate positions
    float3 viewPos = GetViewPosition(i.UV, depth);
    float3 worldPos = GetWorldPosition(i.UV, depth);

    // Add simple directional lighting
    float3 directionalLightColor = float3(1.0f, 1.0f, 1.0f);

    float3 diffuseColor = gbuffer.Color.rgb * CalculateLightingSimple(gbuffer.Normal, LightDirection, directionalLightColor, 1.0f);
    diffuseColor += gbuffer.Light.rgb;

    return float4(diffuseColor, 1.0f);
}

pass Default
{
    BindDefaultConstantBuffer(CombinePSMain);

    SetProfile(5.0);
    SetVertexShader(QuadVSMain);
    SetPixelShader(CombinePSMain);
}
