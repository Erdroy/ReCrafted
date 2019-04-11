// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#ifndef GBUFFER_HLSLI
#define GBUFFER_HLSLI

#ifdef USE_GBUFFERSAMPLING
SamplerState<LinearClamped> GBufferSampler : register(s0);

Texture2D GBufferT0 : register(t0); // Color
Texture2D GBufferT1 : register(t1); // Normal
Texture2D GBufferT2 : register(t2); // Light, Emission
Texture2D Depth : register(t3); // Depth

#define SAMPLE_GBUFFER(tex, uv) tex.Sample(GBufferSampler, uv)
#endif // USE_GBUFFERSAMPLING

struct GBufferOutput 
{
    float4 Color : SV_Target0; // [RGBA(color)]
    float4 Normal : SV_Target1; // [RGB(normal), A(FREE)]
    float4 Light : SV_Target2; // [RGB(light), A(emission)]

#ifdef USE_LOGZBUFFER
    float Depth : SV_Depth;
#endif
};

struct GBuffer
{
    float4 Color;
    float3 Normal;
    float3 Light;
    float Emission;
    float3 ViewPos;
    float3 WorldPos;
    float Depth;
};

#ifdef USE_GBUFFERSAMPLING
GBuffer SampleGBuffer(float2 uv) 
{
    GBuffer gbuffer = (GBuffer)0;

    float4 lightEmission = SAMPLE_GBUFFER(GBufferT2, uv).rgba;

    gbuffer.Color = SAMPLE_GBUFFER(GBufferT0, uv).rgba;
    gbuffer.Normal = DecodeNormal(SAMPLE_GBUFFER(GBufferT1, uv).rgb);
    gbuffer.Light = lightEmission.rgb;
    gbuffer.Emission = lightEmission.a;
    gbuffer.Depth = SAMPLE_GBUFFER(Depth, uv).r;

    //gbuffer.ViewPos = 0; // TODO: Calculate ViewPos
    //gbuffer.WorldPos = 0;  // TODO: Calculate WorldPos

    return gbuffer;
}
#endif // USE_GBUFFERSAMPLING

#endif // GBUFFER_HLSLI
