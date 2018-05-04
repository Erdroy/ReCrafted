// ReCrafted (c) 2016-2018 Always Too Late

#ifndef FULLSCREENQUAD_HLSLI
#define FULLSCREENQUAD_HLSLI

struct QuadVS
{
    float2 Position : POSITION;
    float2 UV : TEXCOORD0;
};

struct QuadPS
{
    float2 Position : SV_POSITION;
    float2 UV : TEXCOORD0;
};

/// <summary>
/// Vertex Shader Function
/// QuadVSMain
/// </summary>
void QuadVSMain(in QuadVS i, out QuadPS o) 
{
    o.Position = TransformPosition2D(i.Position);
    o.UV = i.UV;
}

#endif // FULLSCREENQUAD_HLSLI
