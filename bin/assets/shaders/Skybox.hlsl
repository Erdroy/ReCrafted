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
	float4 color : COLOR;
};

VSOutput VSMain(in VSInput input)
{
	VSOutput output = (VSOutput)0;

	output.position = mul(float4(input.position, 1.0f), WVP);
	output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	return output;
}

float4 PSMain(in VSOutput input) : SV_TARGET
{
	return float4(0.75f, 0.75f, 0.75f, 0.7f);
}