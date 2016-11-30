// ReCrafted © 2016 Damian 'Erdroy' Korczowski

cbuffer Data : register(b0)
{
	matrix WVP;
}

struct VSInput
{
	float3 position : POSITION;
	float2 texcoord : TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

Texture2D m_atlas : register(t0);
SamplerState m_sampler : register(s0);

VSOutput VSMain(in VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.position = mul(float4(input.position, 1.0f), WVP);
	output.texcoord = input.texcoord;
	output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	return output;
}

float4 PSMain(in VSOutput input) : SV_TARGET
{
	return m_atlas.Sample(m_sampler, input.texcoord) * input.color;
}