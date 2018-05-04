// ReCrafted (c) 2016-2018 Always Too Late

#name "Blit"
#desc "Blit shader"

Texture2D Texture : register(t0);
SamplerState<LinearClamped> Sampler : register(s0);

struct VSInput
{
	float2 Position : POSITION;
    float2 TexCoord : TEXCOORD0;
};

struct VSOutput
{
	float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD;
};

/// <summary>
/// Vertex Shader Function
/// BlitVSMain
/// </summary>
void BlitVSMain(in VSInput i, out VSOutput o)
{
	o.Position = float4(i.Position, 0.0f, 1.0f);
	o.TexCoord = i.TexCoord;
}

/// <summary>
/// Pixel Shader Function
/// BlitPSMain
/// </summary>
void BlitPSMain(in VSOutput input, out float4 color) : SV_Target0
{
	//color = float4(input.TexCoord, 0.0f, 1.0f);
    color = Texture.Sample(Sampler, input.TexCoord);
}

pass Default
{
	SetProfile(5.0);
	SetVertexShader(BlitVSMain);
	SetPixelShader(BlitPSMain);
}
