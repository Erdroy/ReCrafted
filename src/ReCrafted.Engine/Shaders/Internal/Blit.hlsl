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

VSOutput VSMain(VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.Position = float4(input.Position, 0.0f, 1.0f);
	output.TexCoord = input.TexCoord;
	return output;
}

float4 PSMain(VSOutput input) : SV_Target0
{
	//return float4(input.TexCoord, 0.0f, 1.0f);
	return Texture.Sample(Sampler, input.TexCoord);
}

pass Default
{
	SetProfile(5.0);
	SetVertexShader(VSMain);
	SetPixelShader(PSMain);
}