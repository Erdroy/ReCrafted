// ReCrafted © 2016 Damian 'Erdroy' Korczowski

cbuffer Data : register(b0)
{
	matrix WVP;
}

struct VSInput
{
	float3 position : POSITION;
};

struct VSOutput
{
	float4 position : SV_POSITION;
};

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