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
Texture2D<float4> AmbientOcculusion : register(t2);
Texture2D<float> Depth : register(t3);
Texture2D<float> ShadowOcculusion : register(t4);

RWTexture2D<float4> OutputTexture : register(u0);

[numthreads(TileSize, TileSize, 1)]
void CSMain(uint3 GroupID : SV_GroupID, uint3 GroupThreadID : SV_GroupThreadID)
{
	uint2 pixelCoord = GroupID.xy * uint2(TileSize, TileSize) + GroupThreadID.xy;

	float4 albedo = Albedo[pixelCoord];
	float4 normal = Normals[pixelCoord] * 2.0f - 1.0f;
	float depth = Depth[pixelCoord];
	float shadow = ShadowOcculusion[pixelCoord];

	// sync
	GroupMemoryBarrierWithGroupSync();

	// calculate dot(n, l)
	float3 lightDir = normalize(LightDir.xyz);
	float3 nDotL = dot(normal, lightDir);
	nDotL = clamp(nDotL, 0.0f, 1.0f);

	// calculate lighting
	float3 lighting = (LightColor.xyz * nDotL) * shadow + float4(0.6f, 0.6f, 0.6f, 0.0f);

	// do the final color
	float4 color = float4(albedo.rgb * lighting, 1.0f);
	
	// apply some effects? Like color grading etc. cuz this is the best place atm

	OutputTexture[pixelCoord] = color;
}