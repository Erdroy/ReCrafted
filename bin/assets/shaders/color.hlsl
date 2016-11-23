cbuffer Data : register(b0)
{
	matrix WVP;
}

struct VSInput
{
	float3 position : POSITION;
	float4 color : COLOR;
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
	output.color = input.color;
	return output;
}

float4 PSMain(in VSOutput input) : SV_TARGET
{
	return input.color;
}