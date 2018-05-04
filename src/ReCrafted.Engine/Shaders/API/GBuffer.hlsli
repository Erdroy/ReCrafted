// ReCrafted (c) 2016-2018 Always Too Late

#ifndef GBUFFER_HLSLI
#define GBUFFER_HLSLI

#ifdef USE_GBUFFERSAMPLING
SamplerState<LinearClamped> GBufferSampler : register(s0);

Texture2D GBufferT0 : register(t0); // Color
Texture2D GBufferT1 : register(t1); // Normal

#define SAMPLE_GBUFFER(tex, uv) tex.Sample(GBufferSampler, uv)
#endif // USE_GBUFFERSAMPLING

struct GBufferOutput 
{
    float4 Color : SV_Target0;
    float4 Normal : SV_Target1; // rgb, A/Z channel/component is still free
};

struct GBuffer
{
    float3 Color;
    float3 Normal;
    float3 ViewPos;
    float3 WorldPos;
};

#ifdef USE_GBUFFERSAMPLING
GBuffer SampleGBuffer(float2 uv) 
{
    GBuffer gbuffer = (GBuffer)0;

    gbuffer.Color = SAMPLE_GBUFFER(GBufferT0, uv).rgb;
    gbuffer.Normal = SAMPLE_GBUFFER(GBufferT1, uv).rgb;
    //gbuffer.ViewPos = 0; // TODO: Calculate ViewPos
    //gbuffer.WorldPos = 0;  // TODO: Calculate WorldPos

    return gbuffer;
}
#endif // USE_GBUFFERSAMPLING

#endif // GBUFFER_HLSLI
