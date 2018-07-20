// ReCrafted (c) 2016-2018 Always Too Late

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
    float3 diffuseColor = gbuffer.Color;
    float3 ambientLightColor = float3(0.1f, 0.1f, 0.1f);
    float3 directionalLightColor = float3(1.0f, 1.0f, 1.0f);

    float3 surfaceNormal = DecodeNormal(gbuffer.Normal);

    diffuseColor *= CalculateLightingSimple(surfaceNormal, LightDirection, ambientLightColor, directionalLightColor, 0.4f);

    return float4(diffuseColor, 1.0f);
}

pass Default
{
    SetProfile(5.0);
    SetDefaultCBTargets(CombinePSMain);

    SetVertexShader(QuadVSMain);
    SetPixelShader(CombinePSMain);
}
