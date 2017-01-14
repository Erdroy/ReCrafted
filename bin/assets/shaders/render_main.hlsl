// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

#define PS_uv
#include "Common.hlsli"

cbuffer Data : register(b0)
{
	float4 LightColor;
	float4 LightDir;
};

Texture2D<float4> Albedo : register(t0);
Texture2D<float4> Normals : register(t1);
Texture2D<float4> AmbientOcculusion : register(t2);
Texture2D<float> ShadowOcculusion : register(t3);

SamplerState PointSampler : register(s0);

VertexShader_Output VSMain(uint vert : SV_VERTEXID)
{
	VertexShader_Output output = (VertexShader_Output)0;

	float2 texcoord = float2(vert & 1, vert >> 1);
	float4 position = float4((texcoord.x - 0.5f) * 2, -(texcoord.y - 0.5f) * 2, 0.0f, 1.0f);

	output.position = position;
	output.uv = texcoord;

	return output;
}

float4 PSMain(PixelShader_Input input) : SV_Target
{
	// sample all
	float4 albedo = Albedo.Sample(PointSampler, input.uv);
	float4 normal = Normals.Sample(PointSampler, input.uv) * 2.0f - 1.0f;
	float shadow = ShadowOcculusion.Sample(PointSampler, input.uv);
	float ambient = AmbientOcculusion.Sample(PointSampler, input.uv);

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
	return float4(albedo.rgb * lighting * ambient, 1.0f);
}