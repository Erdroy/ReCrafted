// ReCrafted (c) 2016-2019 Always Too Late

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

struct SurfaceVSOutput
{
    float4 Position : SV_POSITION;
    float3 Normal   : NORMAL;

#ifdef USE_LOGZBUFFER
    float LogZ : TEXCOORD0;
#endif

#ifdef USE_UV
    float2 UV       : TEXCOORD0;
#endif // USE_UV

#ifdef USE_VERTEXCOLOR
    float4 Color    : COLOR0;
#endif // USE_VERTEXCOLOR
};

// Add macro for SurfacePSInput
#define SurfacePSInput SurfaceVSOutput

/// <summary>
/// Vertex Shader Function
/// SurfaceVSMain
/// </summary>
void SurfaceVSMain(in SurfaceVSInput i, out SurfaceVSOutput o)
{
    float4 position = TransformPosition(i.Position);

    o.Position = position;
    o.Normal = i.Normal;

#ifdef USE_UV
    o.UV = i.UV;
#endif // USE_UV

#ifdef USE_VERTEXCOLOR
    o.Color = i.Color;
#endif // USE_VERTEXCOLOR

#ifdef USE_LOGZBUFFER
    const float C = 1.0f;
    const float FC = 1.0 / log(ViewInfo.y * C + 1);

    o.LogZ = log(position.z * C + 1) * FC; 
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
    o.Normal = EncodeNormal(float4(i.Normal, 1.0f));
    
    // Set Light as simple ambient light
    o.Light = float4(i.Color.rgb * AmbientLightColor, 0.0f);

#ifdef USE_LOGZBUFFER
    // TODO: Make option to use inv z (1.0f - Z/W) instead of log z,
    // to allow hardware optimizations.
    o.Depth = i.LogZ; 
#endif
}

#endif // SURFACE_HLSLI
