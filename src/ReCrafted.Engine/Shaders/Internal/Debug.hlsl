// ReCrafted (c) 2016-2018 Always Too Late

#name "Debug"
#desc "Debug drawing shader"

#define USE_3DSPACE
#include "../ShaderAPI.hlsli"

struct VSInput
{
    float3 Position : POSITION;
    float4 Color : COLOR;
};

struct VSOutput
{
    float4 Position : SV_Position;
    float4 Color : COLOR;
};

/// <summary>
/// Vertex Shader Function
/// VSMain
/// </summary>
void VSMain(in VSInput i, out VSOutput o)
{
    float4 position = TransformPosition(i.Position);

    o.Position = position;
    o.Color = i.Color;
}

/// <summary>
/// Pixel Shader Function
/// PSMain
/// </summary>
void PSMain(in VSOutput i, out float4 color : SV_Target0)
{
    color = i.Color;
}

pass Default
{
    SetProfile(5.0);
    SetVertexShader(VSMain);
    SetPixelShader(PSMain);
}
