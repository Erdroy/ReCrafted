// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#ifndef FULLSCREENQUAD_HLSLI
#define FULLSCREENQUAD_HLSLI

#ifdef USE_FULLSCREENQUAD
struct QuadVS
{
    float2 Position : POSITION;
    float2 UV : TEXCOORD0;
};

struct QuadPS
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD0;
};

/// <summary>
/// Vertex Shader Function
/// QuadVSMain
/// </summary>
void QuadVSMain(in QuadVS i, out QuadPS o) 
{
    o.Position = float4(i.Position, 0.0f, 1.0f);
    o.UV = i.UV;
}
#endif // USE_FULLSCREENQUAD

#endif // FULLSCREENQUAD_HLSLI
