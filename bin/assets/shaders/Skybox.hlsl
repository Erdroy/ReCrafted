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
	float3 worldPos : TEXCOORD;
};

VSOutput VSMain(in VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.position = mul(WVP, float4(input.position, 1.0f));
	output.worldPos = input.position;
	return output;
}

float4 PSMain(in VSOutput input) : SV_TARGET
{
	if (input.worldPos.y > -0.15f)
		return float4(0.0f, 1.0f, 1.0f, 1.0f);

	return float4(0.0f, 0.3f, 0.3f, 1.0f);
}