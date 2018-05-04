// ReCrafted (c) 2016-2018 Always Too Late

#name "GBuffer Combine"
#desc "GBuffer combine shader"

#define USE_GBUFFERSAMPLING
#include "../ShaderAPI.hlsli"

/// <summary>
/// Pixel Shader Function
/// CombinePSMain
/// </summary>
float4 CombinePSMain(in QuadPS i)
{
    GBuffer gbuffer = SampleGBuffer(i.uv);

    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}

pass Default
{
    SetProfile(5.0);
    SetVertexShader(QuadVSMain);
    SetPixelShader(CombinePSMain);
}
