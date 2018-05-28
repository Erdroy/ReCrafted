// ReCrafted (c) 2016-2018 Always Too Late

#ifndef SURFACE_HLSLI
#define SURFACE_HLSLI

struct SurfaceVSInput
{
    float3 Position : POSITION;
    float3 Normal   : NORMAL;

#ifdef USE_UV
    float2 UV       : TEXCOORD0;
#endif // USE_UV

#ifdef USE_VERTEXCOLOR
    float4 Color    : COLOR0;
#endif // USE_VERTEXCOLOR
};

struct SurfacePSInput
{
    float4 Position : SV_POSITION;
    float3 Normal   : NORMAL;

    float LogZ : TEXCOORD0;

#ifdef USE_UV
    float2 UV       : TEXCOORD0;
#endif // USE_UV

#ifdef USE_VERTEXCOLOR
    float4 Color    : COLOR0;
#endif // USE_VERTEXCOLOR
};

/// <summary>
/// Vertex Shader Function
/// SurfaceVSMain
/// </summary>
void SurfaceVSMain(in SurfaceVSInput i, out SurfacePSInput o)
{
    float4 position = TransformPosition(i.Position);

    o.Position = position;
    o.Normal = EncodeNormal(i.Normal);

#ifdef USE_UV
    o.UV = i.UV;
#endif // USE_UV

#ifdef USE_VERTEXCOLOR
    o.Color = i.Color;
#endif // USE_VERTEXCOLOR

#ifdef USE_LOGZBUFFER
    const float C = 1.0f;
    const float FC = 1.0 / log(farPlane * C + 1);

    o.LogZ = log(position.w * C + 1) * FC; 
    o.Position.z = o.LogZ * position.w;
#endif
}

/// <summary>
/// Pixel Shader Function
/// SurfacePSMain
/// </summary>
void SurfacePSMain(in SurfacePSInput i, out GBufferOutput o)
{
    // TODO: Consider whenever we need to use texture sampling, raw vertex color or textureColor * vertexColor
    o.Color = i.Color;
    o.Normal = float4(i.Normal, 1.0f);

#ifdef USE_LOGZBUFFER
    o.Depth = i.LogZ;
#endif
}

#endif // SURFACE_HLSLI
