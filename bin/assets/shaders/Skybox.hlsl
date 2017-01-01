// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

cbuffer Data : register(b0)
{
	matrix WVP;

	float4 ColorUpper;
	float4 ColorMiddle;
	float4 ColorLower;
}

#define HORI_HEIGHT -0.15f

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
	// the colors
	float3 color = float3(0.0f, 0.0f, 0.0f);

	// the height
	float height = input.worldPos.y;

	// calculate distance from the horizon
	float dist = abs(HORI_HEIGHT - height);

	// create interpolation factor
	float interpolation = clamp(dist, 0.0f, 1.0f);

	// interpolate colors
	if (height < HORI_HEIGHT)
	{
		color = lerp(ColorMiddle, ColorLower, interpolation);
	}
	else 
	{
		color = lerp(ColorMiddle, ColorUpper, interpolation);
	}

	// return color
	return float4(color, 1.0f);
}