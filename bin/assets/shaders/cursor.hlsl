// ReCrafted © 2016 Damian 'Erdroy' Korczowski

#define VS_P
#define PS_P
#include "Common.hlsli"

cbuffer Data : register(b0)
{
	matrix WVP;
}

VSOutput VSMain(in VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.position = mul(float4(input.position, 1.0f), WVP);
	return output;
}

float4 PSMain(in VSOutput input) : SV_TARGET
{
	return float4(0.0f, 0.0f, 0.0f, 0.7f);
}