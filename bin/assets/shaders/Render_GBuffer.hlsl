// ReCrafted © 2016 Damian 'Erdroy' Korczowski

#include "Common.hlsli"

cbuffer Data : register(b0)
{
	matrix WVP;
}

struct VSInput
{
	float3 position : POSITION;
	//float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	//float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

Texture2D m_texture : register(t0);
SamplerState m_sampler : register(s0);

VSOutput VSMain(in VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.position = mul(float4(input.position, 1.0f), WVP);
	//output.normal = input.normal;
	output.uv = input.uv;

	return output;
}

GBuffer PSMain(in VSOutput input)
{
	GBuffer gbuffer = (GBuffer)0;

	gbuffer.Albedo = m_texture.Sample(m_sampler, input.uv);
	//gbuffer.Normal = input.normal;

	return gbuffer;
}