// ReCrafted (c) 2016-2018 Always Too Late

#name "TerrainShader"
#desc "Terrain Shader - Used for drawing planetary terrain. Implements own GeometryShader which adds more LowPoly effect."

//#define USE_LOGZBUFFER
#include "../ShaderAPI.hlsli"

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
    const float3 position = i.LocalPosition;
    const float3 triplanarNormal = normalize(i.Normal);
  
    // Blending factor of triplanar mapping
    float3 blend = pow(triplanarNormal.xyz, 8); // TODO: Improve blend function
    blend /= dot(blend, float3(1, 1, 1));

    // Triplanar mapping
    float2 tx = position.zy * 0.5f;
    float2 ty = position.xz * 0.5f;
    float2 tz = position.xy * 0.5f;

    float mat0 = i.Materials0.r;
    float mat1 = i.Materials0.g;
    float mat2 = i.Materials0.b;
    float mat3 = i.Materials0.a;

    float3 color = float3(0.0f, 0.0f, 0.0f);

    if (mat0 > 0.01f) 
    {
        color += (m_texture0.Sample(Sampler, tx).rgb * blend.x + m_texture0.Sample(Sampler, ty).rgb * blend.y + m_texture0.Sample(Sampler, tz).rgb * blend.z) * mat0;
    }

    if (mat1 > 0.01f) 
    {
        color += (m_texture1.Sample(Sampler, tx).rgb * blend.x + m_texture1.Sample(Sampler, ty).rgb * blend.y + m_texture1.Sample(Sampler, tz).rgb * blend.z) * mat1;
    }

    if (mat2 > 0.01f) 
    {
        color += (m_texture2.Sample(Sampler, tx).rgb * blend.x + m_texture2.Sample(Sampler, ty).rgb * blend.y + m_texture2.Sample(Sampler, tz).rgb * blend.z) * mat2;
    }

    if (mat3 > 0.01f)
    {
        color += (m_texture3.Sample(Sampler, tx).rgb * blend.x + m_texture3.Sample(Sampler, ty).rgb * blend.y + m_texture3.Sample(Sampler, tz).rgb * blend.z) * mat3;
    }

    o.Color = float4(color, 1.0f);

    o.Normal = EncodeNormal(float4(i.Normal, 1.0f));

    // Set Light as simple ambient light
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