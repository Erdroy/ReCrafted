// ReCrafted © 2016 Damian 'Erdroy' Korczowski

struct GBuffer
{
	float4 Albedo : SV_Target0;
	float3 Normal : SV_Target1;
};

#ifdef VS_PTN
struct VSInput
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};
#endif

#ifdef PS_PTN
struct VSOutput
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};
#endif

float3 PositionFromDepth(in float zw, in uint2 pixelCoord, float2 screenSize, matrix invViewProj)
{
	float2 cpos = (pixelCoord + 0.5f) / screenSize;

	cpos *= 2.0f;
	cpos -= 1.0f;
	cpos.y *= -1.0f;

	float4 position = mul(float4(cpos, zw, 1.0f), invViewProj);

	return position.xyz / position.w;
}

// https://mynameismjp.wordpress.com/2009/03/10/reconstructing-position-from-depth/
float3 PositionFromDepth(float2 depth, float2 vTexCoord, matrix invProj)
{
	// Get the depth value for this pixel
	float z = depth;
	// Get x/w and y/w from the viewport position
	float x = vTexCoord.x * 2 - 1;
	float y = (1 - vTexCoord.y) * 2 - 1;
	float4 vProjectedPos = float4(x, y, z, 1.0f);
	// Transform by the inverse projection matrix
	float4 vPositionVS = mul(vProjectedPos, invProj);
	// Divide by w to get the view-space position
	return vPositionVS.xyz / vPositionVS.w;
}