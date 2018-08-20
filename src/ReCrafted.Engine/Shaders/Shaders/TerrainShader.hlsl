// ReCrafted (c) 2016-2018 Always Too Late

#name "TerrainShader"
#desc "Terrain Shader - Used for drawing planetary terrain. Implements own GeometryShader which adds more LowPoly effect."

#define USE_LOGZBUFFER
#include "../ShaderAPI.hlsli"

Texture2D m_texture0 : register(t0);
Texture2D m_texture1 : register(t1);

SamplerState<LinearWrap> Sampler : register(s0);

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
    float3 Normal   : NORMAL0;
    float3 FlatNormal : NORMAL1;
    float3 FixedNormal : NORMAL2;
    float4 Color    : COLOR0;
    uint2 Materials : TEXCOORD0;
    float Blend : TEXCOORD1;

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
    o.Color = i.Color;
    o.Materials = i.Materials;
    o.Blend = ((i.Materials[0] >> 8) & 0xFF) / 255.f;

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
    const float3 pointNormal = normalize(position - origin);
    const float3 baseYNormal = float3(0.0f, upSign, 0.0f);

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
    output.FlatNormal = normalize(input[0].Normal + input[1].Normal + input[2].Normal);

    // Add 1st triangle
    output.Position = input[0].Position;
    output.LocalPosition = input[0].LocalPosition;
    output.Normal = input[0].Normal;
    output.FixedNormal = Triplanar_CalculateYFixedNormal(sphereOrigin, output.LocalPosition, output.Normal);
    output.Color = input[0].Color;
    output.Materials = input[0].Materials;
    output.Blend = input[0].Blend;
#ifdef USE_LOGZBUFFER
    output.LogZ = input[0].LogZ;
#endif
    OutputStream.Append(output);

    // Add 2nd triangle
    output.Position = input[1].Position;
    output.LocalPosition = input[1].LocalPosition;
    output.Normal = input[1].Normal;
    output.FixedNormal = Triplanar_CalculateYFixedNormal(sphereOrigin, output.LocalPosition, output.Normal);
    output.Color = input[1].Color;
    output.Materials = input[1].Materials;
    output.Blend = input[0].Blend;
#ifdef USE_LOGZBUFFER
    output.LogZ = input[1].LogZ;
#endif
    OutputStream.Append(output);

    // Add 3rd triangle
    output.Position = input[2].Position;
    output.LocalPosition = input[2].LocalPosition;
    output.Normal = input[2].Normal;
    output.FixedNormal = Triplanar_CalculateYFixedNormal(sphereOrigin, output.LocalPosition, output.Normal);
    output.Color = input[2].Color;
    output.Materials = input[2].Materials;
    output.Blend = input[0].Blend;
#ifdef USE_LOGZBUFFER
    output.LogZ = input[2].LogZ;
#endif
    OutputStream.Append(output);

    OutputStream.RestartStrip();
}

float3 TmpGetMatColor(in uint matId) 
{
    switch (matId)
    {
    case 1:
        return float3(0.35f, 0.35f, 0.35f);
    case 2:
        return float3(80 / 255.0f, 145 / 255.0f, 30 / 255.0f);

    case 255:
        return float3(1.0f, 0.0f, 0.0f);

    default:
        return float3(0.1f, 0.0f, 0.1f);
    }
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
    float3 blend = pow(triplanarNormal.xyz, 16); // TODO: Improve blend function
    blend /= dot(blend, float3(1, 1, 1));

    // Triplanar mapping
    float2 tx = position.yz * 1.0f;
    float2 ty = position.zx * 1.0f;
    float2 tz = position.xy * 1.0f;

    // Txz, Uxz
    float2 material1 = float2(i.Materials[0] >> 24, (i.Materials[0] >> 16) & 0xFF);

    // Tpy, Tny, Upy, Uny
    float4 material2 = float4(i.Materials[1] >> 24, (i.Materials[1] >> 16) & 0xFF, (i.Materials[1] >> 8) & 0xFF, (i.Materials[1]) & 0xFF);

    float3 flip = float3(triplanarNormal.x < 0.0, triplanarNormal.y < 0.0, triplanarNormal.z >= 0.0);
    float2 zindex = lerp(material2.xz, material2.yw, flip.y);

    float3 cx0 = TmpGetMatColor(material1.x);
    float3 cz0 = TmpGetMatColor(material1.x);
    float3 cy0 = TmpGetMatColor(zindex.x);

    float3 cx1 = TmpGetMatColor(material1.y);
    float3 cz1 = TmpGetMatColor(material1.y);
    float3 cy1 = TmpGetMatColor(zindex.y);

    float3 albedo = lerp((blend.x * cx0 + blend.y * cy0 + blend.z * cz0), (blend.x * cx1 + blend.y * cy1 + blend.z * cz1), i.Blend);

    o.Color = float4(albedo, 1.0f);

    o.Normal = EncodeNormal(float4(i.FlatNormal, 1.0f));

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