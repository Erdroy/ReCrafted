// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

cbuffer Data : register(b0)
{
	matrix WorldViewProjection;
}

struct VSInput
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float depth : TEXCOORD0;
};

VSOutput VSMain(in VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.position = mul(float4(input.position, 1.0f), WorldViewProjection);
	output.depth = 1.0f - (output.position.z / output.position.w);
	return output;
}

float PSMain(in VSOutput input) : SV_TARGET
{
	return input.depth;
}