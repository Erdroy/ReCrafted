// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

#include "../Common.hlsli"

cbuffer Data : register(b0) {

	matrix g_matInvView;
	matrix g_matInvProj;

	matrix g_matLightViewProj;
	float2 g_vShadowMapSize;
	float2 g_vOcclusionTextureSize;
}

struct VSOutput
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

Texture2D ShadowMap : register(t0);
Texture2D Depth : register(t1);

SamplerState Sampler : register(s0);

static const float BIAS = 0.00056f;

float CalcShadowPCF(float fLightDepth, float2 vShadowTexCoord)
{
	float fShadowTerm = 0.0f;

	float2 vShadowMapCoord = g_vShadowMapSize * vShadowTexCoord;
         
	float2 vLerps = frac(vShadowMapCoord);

	float fSamples[4];
	fSamples[0] = (ShadowMap.Sample(Sampler, vShadowTexCoord).x + BIAS < fLightDepth) ? 0.0f : 1.0f;
	fSamples[1] = (ShadowMap.Sample(Sampler, vShadowTexCoord + float2(1.0 / g_vShadowMapSize.x, 0)).x + BIAS < fLightDepth) ? 0.0f : 1.0f;
	fSamples[2] = (ShadowMap.Sample(Sampler, vShadowTexCoord + float2(0, 1.0 / g_vShadowMapSize.y)).x + BIAS < fLightDepth) ? 0.0f : 1.0f;
	fSamples[3] = (ShadowMap.Sample(Sampler, vShadowTexCoord + float2(1.0 / g_vShadowMapSize.x, 1.0 / g_vShadowMapSize.y)).x + BIAS < fLightDepth) ? 0.0f : 1.0f;

	fShadowTerm = lerp(lerp(fSamples[0], fSamples[1], vLerps.x), lerp(fSamples[2], fSamples[3], vLerps.x), vLerps.y);

	return fShadowTerm;
}

float CalcShadowSoftPCF(float fLightDepth, float2 vShadowTexCoord, int iSqrtSamples)
{
	float fShadowTerm = 0.0f;

	float fRadius = (iSqrtSamples - 1.0f) / 2;
	float fWeightAccum = 0.0f;

	[unroll]
	for (float y = -fRadius; y <= fRadius; y++)
	{
		[unroll]
		for (float x = -fRadius; x <= fRadius; x++)
		{
			float2 vOffset = 0;
			vOffset = float2(x, y);
			vOffset /= g_vShadowMapSize;
			float2 vSamplePoint = vShadowTexCoord + vOffset;
			float fDepth = ShadowMap.Sample(Sampler, vSamplePoint).x;
			float fSample = (fLightDepth <= fDepth + BIAS);

			// edge tap smoothing
			float xWeight = 1;
			float yWeight = 1;

			if (x == -fRadius)
				xWeight = 1 - frac(vShadowTexCoord.x * g_vShadowMapSize.x);
			else if (x == fRadius)
				xWeight = frac(vShadowTexCoord.x * g_vShadowMapSize.x);

			if (y == -fRadius)
				yWeight = 1 - frac(vShadowTexCoord.y * g_vShadowMapSize.y);
			else if (y == fRadius)
				yWeight = frac(vShadowTexCoord.y * g_vShadowMapSize.y);

			fShadowTerm += fSample * xWeight * yWeight;
			fWeightAccum = xWeight * yWeight;
		}
	}

	fShadowTerm /= (iSqrtSamples * iSqrtSamples);
	fShadowTerm *= 1.55f;

	return fShadowTerm;
}

VSOutput VSMain(in float3 position : POSITION, in float2 uv : TEXCOORD)
{
	VSOutput output = (VSOutput)0;

	output.position = float4(position, 1.0f);
	output.uv = uv;

	return output;
}

float PSMain(in VSOutput input) : SV_TARGET
{
	float depth = Depth.Sample(Sampler, input.uv).r;
	float4 vPositionVS = float4(PositionFromDepth(depth, input.uv, g_matInvProj), 1.0f);
	
	[branch]
	if (depth >= 0.99999f) // limit depth(ignore faarrr skybox etc.)
		return 1.0f;

	// TODO: max shadow distance

	float4x4 matViewToLightViewProj = mul(g_matInvView, g_matLightViewProj);
	float4 vPositionLightCS = mul(vPositionVS, matViewToLightViewProj);

	float fLightDepth = vPositionLightCS.z / vPositionLightCS.w;

	float2 vShadowTexCoord = 0.5 * vPositionLightCS.xy / vPositionLightCS.w + float2(0.5f, 0.5f);
	vShadowTexCoord.y = 1.0f - vShadowTexCoord.y;

	vShadowTexCoord += (0.5f / g_vShadowMapSize);

	// get the shadow occlusion factor
	return CalcShadowSoftPCF(fLightDepth, vShadowTexCoord, 3); // 3x3
}