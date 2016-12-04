// ReCrafted © 2016 Damian 'Erdroy' Korczowski

#include "Common.hlsli"

static const uint LightTileSize = 16;

Texture2D<float4> Albedo : register(t0);
Texture2D<float4> Normals : register(t1);

RWTexture2D<float4> OutputTexture : register(u0);

[numthreads(LightTileSize, LightTileSize, 1)]
void CSMain(uint3 GroupID : SV_GroupID, uint3 GroupThreadID : SV_GroupThreadID)
{
	uint2 pixelCoord = GroupID.xy * uint2(LightTileSize, LightTileSize) + GroupThreadID.xy;

	// sync
	GroupMemoryBarrierWithGroupSync();

	OutputTexture[pixelCoord] = Albedo.Load(int3(pixelCoord, 0));
}