// ReCrafted © 2016 Damian 'Erdroy' Korczowski

#define VS_PT
#define PS_PT

#include "Common.hlsli"

cbuffer Data : register(b0)
{
	matrix WVP;
}

Texture2D m_texture : register(t0);
SamplerState m_sampler : register(s0);

VSOutput VSMain(in VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.position = mul(float4(input.position, 1.0f), WVP);
	output.uv = input.uv;
	//output.normal = input.normal;

	return output;
}

GBuffer PSMain(in VSOutput input)
{
	GBuffer gbuffer = (GBuffer)0;

	gbuffer.Albedo = m_texture.Sample(m_sampler, input.uv);
	//gbuffer.Normal = input.normal;

	return gbuffer;
}