// ReCrafted (c) 2016-2018 Always Too Late

#ifndef POSTPROCESSING_HLSLI
#define POSTPROCESSING_HLSLI

Texture2D T0 : register(t0);
Texture2D T1 : register(t0);
SamplerState<LinearClamped> Sampler : register(s0);

struct VSInput
{
    float2 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
};

struct VSOutput
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD;
};

float4 PostProcessMain(in float4 color, in float2 uv, in float depth);

/// <summary>
/// Vertex Shader Function
/// BlitVSMain
/// </summary>
void PostProcessingVSMain(in VSInput i, out VSOutput o)
{
    o.Position = float4(i.Position, 0.0f, 1.0f);
    o.TexCoord = i.TexCoord;
}

/// <summary>
/// Pixel Shader Function
/// BlitPSMain
/// </summary>
float4 PostProcessingPSMain(in VSOutput input) : SV_Target0
{
    float4 color = T0.Sample(Sampler, input.TexCoord).rgba;
    float depth = T1.Sample(Sampler, input.TexCoord).r;

    return PostProcessMain(color, input.TexCoord, depth);
}

#endif // POSTPROCESSING_HLSLI