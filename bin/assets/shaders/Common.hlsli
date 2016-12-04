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