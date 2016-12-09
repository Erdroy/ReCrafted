// ReCrafted © 2016 Damian 'Erdroy' Korczowski

#define CS_GGT

#include "Common.hlsli"

static const uint TileSize = 16;

Texture2D<float4> Albedo : register(t0);
Texture2D<float4> Normals : register(t1);

RWTexture2D<float4> OutputTexture : register(u0);

[numthreads(TileSize, TileSize, 1)]
void CSMain(uint3 GroupID : SV_GroupID, uint3 GroupThreadID : SV_GroupThreadID)
{
	uint2 pixelCoord = GroupID.xy * uint2(TileSize, TileSize) + GroupThreadID.xy;

	float4 albedo = Albedo[pixelCoord];
	float4 normal = Normals[pixelCoord] * 2.0f - 1.0f;

	// sync
	GroupMemoryBarrierWithGroupSync();

	float3 lightDir = normalize(float3(0.4f, 0.4f, 0.2f));
	lightDir = -lightDir;

	float3 nDotL = dot(normal, lightDir);
	
	float3 lighting = albedo.rgb * nDotL;
	
	OutputTexture[pixelCoord] = float4(albedo.rgb + lighting, 1.0f);
}