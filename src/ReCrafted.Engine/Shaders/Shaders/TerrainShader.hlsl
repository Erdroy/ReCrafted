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
    float3 FlatNormal       : NORMAL1;
    float3 FixedNormal      : NORMAL2;

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
    o.FlatNormal = (float3)0;
    o.FixedNormal = (float3)0;
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

float3 Triplanar_CalculateYFixedNormal(float3 origin, float3 position, float3 normal)
{
    const float upSign = position.y < origin.y ? -1.0f : 1.0f;
    const float3 baseYNormal = float3(0.0f, upSign, 0.0f);
    const float3 pointNormal = normalize(position - origin);

    // Calculate angle
    float3 surfaceUpCross = cross(pointNormal, baseYNormal);
    float angle = asin(length(surfaceUpCross));

    // Calculate angle axis rotation matrix
    float3x3 rotationMatrix = rotateAngleAxis(angle, normalize(surfaceUpCross));

    // Rotate normal around [SurfaceNormal x YpNormal]
    float3 fixedNormal = mul(rotationMatrix, normal);

    // Inverse Y when we are under the origin
    fixedNormal.y *= upSign;

    // Return normal
    return fixedNormal;
}

[maxvertexcount(3)]
void TerrainGSMain(triangle TerrainVSOutput input[3], inout TriangleStream<TerrainPSInput> OutputStream)
{
    const float3 sphereOrigin = float3(0.0f, 0.0f, 0.0f);

    TerrainPSInput output;
    
    // Calculate output normal for all vertices
    output.FlatNormal = normalize(input[0].Normal + input[1].Normal + input[2].Normal);

    // Add 1st vertex
    output.Position = input[0].Position;
    output.LocalPosition = input[0].LocalPosition;
    output.Normal = input[0].Normal;
    output.FixedNormal = Triplanar_CalculateYFixedNormal(sphereOrigin, output.LocalPosition, output.Normal);
    output.Materials0 = input[0].Materials0;
    output.Materials1 = input[0].Materials1;
    output.Materials2 = input[0].Materials2;
    output.Materials3 = input[0].Materials3;
#ifdef USE_LOGZBUFFER
    output.LogZ = input[0].LogZ;
#endif
    OutputStream.Append(output);

    // Add 2nd vertex
    output.Position = input[1].Position;
    output.LocalPosition = input[1].LocalPosition;
    output.Normal = input[1].Normal;
    output.FixedNormal = Triplanar_CalculateYFixedNormal(sphereOrigin, output.LocalPosition, output.Normal);
    output.Materials0 = input[1].Materials0;
    output.Materials1 = input[1].Materials1;
    output.Materials2 = input[1].Materials2;
    output.Materials3 = input[1].Materials3;
#ifdef USE_LOGZBUFFER
    output.LogZ = input[1].LogZ;
#endif
    OutputStream.Append(output);

    // Add 3rd vertex
    output.Position = input[2].Position;
    output.LocalPosition = input[2].LocalPosition;
    output.Normal = input[2].Normal;
    output.FixedNormal = Triplanar_CalculateYFixedNormal(sphereOrigin, output.LocalPosition, output.Normal);
    output.Materials0 = input[2].Materials0;
    output.Materials1 = input[2].Materials1;
    output.Materials2 = input[2].Materials2;
    output.Materials3 = input[2].Materials3;
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
    const float3 position = i.LocalPosition;
    const float3 triplanarNormal = normalize(i.FixedNormal);
  
    // Blending factor of triplanar mapping
    float3 blend = pow(triplanarNormal.xyz, 8); // TODO: Improve blend function
    blend /= dot(blend, float3(1, 1, 1));

    // Triplanar mapping
    float2 tx = position.yz * 0.25f;
    float2 ty = position.zx * 0.25f;
    float2 tz = position.xy * 0.25f;

    // TODO: Rotate UV (tx, ty, tz)

    float mat0 = i.Materials0.r;
    float mat1 = i.Materials0.g;
    float mat2 = i.Materials0.b;
    float mat3 = i.Materials0.a;

    float3 materialA = (m_texture0.Sample(Sampler, tx).rgb * blend.x + m_texture0.Sample(Sampler, ty).rgb * blend.y + m_texture0.Sample(Sampler, tz).rgb * blend.z);
    float3 materialB = (m_texture1.Sample(Sampler, tx).rgb * blend.x + m_texture1.Sample(Sampler, ty).rgb * blend.y + m_texture1.Sample(Sampler, tz).rgb * blend.z);
    float3 materialC = (m_texture2.Sample(Sampler, tx).rgb * blend.x + m_texture2.Sample(Sampler, ty).rgb * blend.y + m_texture2.Sample(Sampler, tz).rgb * blend.z);
    float3 materialD = (m_texture3.Sample(Sampler, tx).rgb * blend.x + m_texture3.Sample(Sampler, ty).rgb * blend.y + m_texture3.Sample(Sampler, tz).rgb * blend.z);

    float3 color = float3(0.0f, 0.0f, 0.0f);
    color += materialA * mat0;
    color += materialB * mat1;
    color += materialC * mat2;
    color += materialD * mat3;

    o.Color = float4(color, 1.0f);

    //o.Normal = EncodeNormal(float4(i.FlatNormal, 1.0f)); // Flat shading
    o.Normal = EncodeNormal(float4(i.Normal, 1.0f)); // Smooth shading

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
    SetGeometryShader(TerrainGSMain);
    SetPixelShader(TerrainPSMain);
}