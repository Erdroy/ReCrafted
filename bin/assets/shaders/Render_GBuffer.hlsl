// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

#define PS_PTNC
#include "Common.hlsli"

cbuffer Data : register(b0)
{
	matrix WVP;
}

Texture2D m_texture : register(t0);
SamplerState m_sampler : register(s0);

VSOutput VSMain(in GBuffer_VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.position = mul(float4(input.position, 1.0f), WVP);
	output.normal = normalize(float4(input.normal, 1.0f)) * 0.5f + 0.5f;
	output.uv = input.uv;
	output.color = input.color;

	return output;
}

GBuffer PSMain(in VSOutput input)
{
	GBuffer gbuffer = (GBuffer)0;

	gbuffer.Albedo = m_texture.Sample(m_sampler, input.uv);
	gbuffer.Normal = float4(input.normal, 1.0f);
	gbuffer.Ambient = input.color.r;

	return gbuffer;
}