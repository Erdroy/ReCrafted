// ReCrafted © 2016 Damian 'Erdroy' Korczowski

#define CS_GGT

#include "Common.hlsli"

static const uint TileSize = 16;

cbuffer Data : register(b0)
{
	float4 LightColor;
	float4 LightDir;
};

Texture2D<float4> Albedo : register(t0);
Texture2D<float4> Normals : register(t1);

RWTexture2D<float4> OutputTexture : register(u0);

float4 AdjustSaturation(float4 color, float saturation) {
	float grey = dot(color, float4(0.3, 0.59, 0.11, 0.0));
	return lerp(grey, color, saturation);
}

[numthreads(TileSize, TileSize, 1)]
void CSMain(uint3 GroupID : SV_GroupID, uint3 GroupThreadID : SV_GroupThreadID)
{
	uint2 pixelCoord = GroupID.xy * uint2(TileSize, TileSize) + GroupThreadID.xy;

	float4 albedo = Albedo[pixelCoord];
	float4 normal = Normals[pixelCoord] * 2.0f - 1.0f;

	// sync
	GroupMemoryBarrierWithGroupSync();

	float3 lightDir = normalize(LightDir.xyz);
	float3 nDotL = dot(normal, lightDir);
	nDotL = clamp(nDotL, 0.0f, 1.0f);

	float3 light = LightColor.xyz * nDotL;
	float3 lighting = light + float4(0.6f, 0.6f, 0.6f, 0.0f);


	float4 color = float4(albedo.rgb * lighting, 1.0f);

	// apply post processing?

	OutputTexture[pixelCoord] = color;
}