// ReCrafted © 2016 Damian 'Erdroy' Korczowski

#include "../Common.hlsli"

cbuffer Data : register(b0)
{
	float4x4 WorldViewProjection;
}

struct VSOutput
{
	float4 position	: SV_POSITION;
	float2 depth : TEXCOORD0;
};

VSOutput VSMain(in GBuffer_VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.position = mul(float4(input.position, 1.0f), WorldViewProjection);
	output.depth = output.position.zw;
	return output;
}

float PSMain(in VSOutput input) : SV_TARGET
{
	return (input.depth.x / input.depth.y);
}