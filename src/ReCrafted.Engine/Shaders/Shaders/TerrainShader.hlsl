// ReCrafted (c) 2016-2018 Always Too Late

#name "TerrainShader"
#desc "Terrain Shader - Used for drawing planetary terrain. Implements own GeometryShader which adds more LowPoly effect."

#define USE_LOGZBUFFER
#include "../ShaderAPI.hlsli"

struct TerrainVSInput
{
    float3 Position : POSITION;
    float3 Normal   : NORMAL;
    float4 Color    : COLOR0;
    uint2 Materials : TEXCOORD0;
};

struct TerrainVSOutput
{
    float4 Position : SV_POSITION;
    float3 LocalPosition : WS_POSITION;
    float3 Normal   : NORMAL;
    float4 Color    : COLOR0;
    uint2 Materials : TEXCOORD0;

#ifdef USE_LOGZBUFFER
    float LogZ : TEXCOORD1;
#endif
};

#define TerrainPSInput TerrainVSOutput

/// <summary>
/// Vertex Shader Function
/// SurfaceVSMain
/// </summary>
void TerrainVSMain(in TerrainVSInput i, out TerrainVSOutput o)
{
    float4 position = TransformPosition(i.Position);

    o.Position = position;
    o.LocalPosition = i.Position;
    o.Normal = i.Normal;
    o.Color = i.Color;
    o.Materials = i.Materials;

    // TODO: Vertex lighting

#ifdef USE_LOGZBUFFER
    const float C = 1.0f;
    const float FC = 1.0 / log(ViewInfo.y * C + 1);

    o.LogZ = log(position.z * C + 1) * FC;
    o.Position.z = o.LogZ * position.w;
#endif
}

[maxvertexcount(3)]
void TerrainGSMain(triangle TerrainVSOutput input[3], inout TriangleStream<TerrainPSInput> OutputStream)
{
    TerrainPSInput output;
    
    // Calculate output normal for all vertices
    output.Normal = normalize(input[0].Normal + input[1].Normal + input[2].Normal);

    // TODO: Calculate flat lighting

    // Add 1st triangle
    output.Position = input[0].Position;
    output.LocalPosition = input[0].LocalPosition;
    output.Color = input[0].Color;
    output.Materials = input[0].Materials;
#ifdef USE_LOGZBUFFER
    output.LogZ = input[0].LogZ;
#endif
    OutputStream.Append(output);

    // Add 2nd triangle
    output.Position = input[1].Position;
    output.LocalPosition = input[1].LocalPosition;
    output.Color = input[1].Color;
    output.Materials = input[1].Materials;
#ifdef USE_LOGZBUFFER
    output.LogZ = input[1].LogZ;
#endif
    OutputStream.Append(output);

    // Add 3rd triangle
    output.Position = input[2].Position;
    output.LocalPosition = input[2].LocalPosition;
    output.Color = input[2].Color;
    output.Materials = input[2].Materials;
#ifdef USE_LOGZBUFFER
    output.LogZ = input[2].LogZ;
#endif
    OutputStream.Append(output);

    OutputStream.RestartStrip();
}

/// <summary>
/// Pixel Shader Function
/// TerrainPSMain
/// </summary>
void TerrainPSMain(in TerrainPSInput i, out GBufferOutput o)
{
    // Blending factor of triplanar mapping
    float3 blend = pow(i.Normal.xyz, 4);
    blend /= dot(blend, float3(1, 1, 1));

    // Triplanar mapping
    float2 tx = i.LocalPosition.yz * 1.0f;
    float2 ty = i.LocalPosition.zx * 1.0f;
    float2 tz = i.LocalPosition.xy * 1.0f;

    float3 colorA = float3(1.0f, 0.0f, 0.0f);
    float3 colorB = float3(0.0f, 1.0f, 0.0f);
    float3 colorC = float3(0.0f, 0.0f, 1.0f);

    //o.Color = float4((colorA * blend.x) + (colorB * blend.y) + (colorC * blend.z), 1.0f);

    o.Color = i.Color;

    // TODO: Consider whenever we need to use texture sampling, raw vertex color or textureColor * vertexColor
    //o.Color = float4(tx, 0.0f, 1.0f);
    o.Normal = EncodeNormal(float4(i.Normal, 1.0f));

    // Set Light as simple ambient light
    o.Light = float4(i.Color.rgb * AmbientLightColor, 0.0f);

#ifdef USE_LOGZBUFFER
    // TODO: Make option to use inv z (1.0f - Z/W) instead of log z,
    // to allow hardware optimizations.
    o.Depth = i.LogZ;
#endif
}

pass Default 
{
    BindDefaultConstantBuffer(SurfaceVSMain, SurfacePSMain);

    SetProfile(5.0);
    SetVertexShader(TerrainVSMain);
    SetGeometryShader(TerrainGSMain);
    SetPixelShader(TerrainPSMain);
}