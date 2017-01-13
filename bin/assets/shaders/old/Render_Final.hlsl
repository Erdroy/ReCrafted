// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

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
	float shadow = ShadowOcculusion[pixelCoord];
	float ambient = AmbientOcculusion[pixelCoord];

	// calculate dot(n, l)
	float3 lightDir = normalize(LightDir.xyz);
	float3 nDotL = dot(normal, lightDir);
	nDotL = clamp(nDotL, 0.0f, 1.0f);

	// calculate ambient
	ambient = 1.0f - ambient;

	// calculate lighting
	float3 lightColor = LightColor.xyz * LightColor.w;
	float3 lighting = (lightColor * nDotL) * shadow + float4(0.6f, 0.6f, 0.6f, 0.0f);

	// do the final color
	float4 color = float4(albedo.rgb * lighting * ambient, 1.0f);
	
	OutputTexture[pixelCoord] = color;
}