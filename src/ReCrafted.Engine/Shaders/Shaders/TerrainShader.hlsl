// ReCrafted (c) 2016-2018 Always Too Late

#name "TerrainShader"
#desc "Terrain Shader - Used for drawing planetary terrain. Implements own GeometryShader which adds more LowPoly effect."

//#define USE_LOGZBUFFER
#include "../ShaderAPI.hlsli"
#include "../Common/TriplanarMapping.hlsli"

Texture2D m_texture0 : register(t0);
Texture2D m_texture1 : register(t1);
Texture2D m_texture2 : register(t2);
Texture2D m_texture3 : register(t3);

SamplerState<LinearWrap> Sampler : register(s0);

struct TerrainVSInput
{
    float3 Position     : POSITION;
    float3 Normal       : NORMAL;

    float4 Materials0   : COLOR0;
    float4 Materials1   : COLOR1;
    float4 Materials2   : COLOR2;
    float4 Materials3   : COLOR3;
};

struct TerrainVSOutput
{
    float4 Position         : SV_POSITION;
    float3 LocalPosition    : WS_POSITION;

    float3 Normal           : NORMAL0;

    float4 Materials0       : COLOR0;
    float4 Materials1       : COLOR1;
    float4 Materials2       : COLOR2;
    float4 Materials3       : COLOR3;

#ifdef USE_LOGZBUFFER
    float LogZ : TEXCOORD2;
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
    o.Materials0 = i.Materials0;
    o.Materials1 = i.Materials1;
    o.Materials2 = i.Materials2;
    o.Materials3 = i.Materials3;

#ifdef USE_LOGZBUFFER
    const float C = 1.0f;
    const float FC = 1.0 / log(ViewInfo.y * C + 1);

    o.LogZ = log(position.z * C + 1) * FC;
    o.Position.z = o.LogZ * position.w;
#endif
}

/// <summary>
/// Pixel Shader Function
/// TerrainPSMain
/// </summary>
void TerrainPSMain(in TerrainPSInput i, out GBufferOutput o)
{
#define AddTextureBlending(tex, mat, scale) \
    if (mat > 0.01f) color += TriplanarSample(tex, Sampler, coords, blendWeights, scale) * mat

    // Calculate coords of triplanar mapping
    const float3 coords = TriplanarCoords(i.LocalPosition);
  
    // Calculate blending weights of triplanar mapping
    const float3 blendWeights = TriplanarWeights(i.Normal.xyz);

    float3 color = float3(0.0f, 0.0f, 0.0f);

    // TODO: Texture array

    AddTextureBlending(m_texture0, i.Materials0.r, 0.5f);
    AddTextureBlending(m_texture1, i.Materials0.g, 0.5f);
    AddTextureBlending(m_texture2, i.Materials0.b, 0.5f);
    AddTextureBlending(m_texture3, i.Materials0.a, 0.5f);

    /*AddTextureBlending(m_texture0, i.Materials1.r, 0.5f);
    AddTextureBlending(m_texture1, i.Materials1.g, 0.5f);
    AddTextureBlending(m_texture2, i.Materials1.b, 0.5f);
    AddTextureBlending(m_texture3, i.Materials1.a, 0.5f);

    AddTextureBlending(m_texture0, i.Materials2.r, 0.5f);
    AddTextureBlending(m_texture1, i.Materials2.g, 0.5f);
    AddTextureBlending(m_texture2, i.Materials2.b, 0.5f);
    AddTextureBlending(m_texture3, i.Materials2.a, 0.5f);

    AddTextureBlending(m_texture0, i.Materials3.r, 0.5f);
    AddTextureBlending(m_texture1, i.Materials3.g, 0.5f);
    AddTextureBlending(m_texture2, i.Materials3.b, 0.5f);
    AddTextureBlending(m_texture3, i.Materials3.a, 0.5f);*/


    // Setup output
    o.Color = float4(color, 1.0f);
    o.Normal = EncodeNormal(float4(i.Normal, 1.0f));
    o.Light = float4(o.Color.rgb * AmbientLightColor, 0.0f);

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
    SetPixelShader(TerrainPSMain);
}