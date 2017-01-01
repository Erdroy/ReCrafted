// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

#define POSTPROCESS_BASE
#include "../Common.hlsli"

Texture2D m_texture : register(t0);
SamplerState m_sampler : register(s0);

float4 PSMain(in PSInput input) : SV_TARGET
{
	float4 color = m_texture.Sample(m_sampler, input.uv);
	float2 dist = input.uv - 0.5f;

	dist.x = 0.5 - dot(dist, dist);
	color *= pow(dist.x, 0.2f);

	return float4(color.rgb, 1.0f);
}