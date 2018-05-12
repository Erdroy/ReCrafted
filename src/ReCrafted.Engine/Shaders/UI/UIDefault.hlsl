// ReCrafted (c) 2016-2018 Always Too Late

#name "UIDefault"
#desc "Default UI shader"

Texture2D Texture : register(t0);
SamplerState<LinearClamped> Sampler : register(s0);

struct VSInput
{
    float2 Position : POSITION;
    float2 UV : TEXCOORD;
    float4 Color : COLOR;
};

struct VSOutput
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    float4 Color : COLOR;
};

/// <summary>
/// Vertex Shader Function
/// VSMain
/// </summary>
void VSMain(in VSInput i, out VSOutput o)
{
    o.Position = float4(i.Position, 0.0f, 1.0f);
    o.UV = i.UV;
    o.Color = i.Color;
}

/// <summary>
/// Pixel Shader Function
/// PSMain
/// </summary>
void PSMain(in VSOutput i, out float4 color : SV_Target0)
{
    if (i.UV.x >= 0)
    {
        color = Texture.Sample(Sampler, i.UV) * i.Color;
        return;
    }

    color = i.Color;
}

pass Default
{
    SetProfile(5.0);
    SetVertexShader(VSMain);
    SetPixelShader(PSMain);
}
