// some test shader

#name "Test"
#desc "Shader for r3d shader compiler test"

Texture2D m_atlas : register(t0);
SamplerState m_sampler : register(s0);

cbuffer Data : register(b0) {
	matrix WVP;
}

struct VSInput
{
	float3 position : POSITION;
	float2 uv : TEXCOORD0;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

VSOutput VSMain(in VSInput input)
{
	VSOutput output = (VSOutput)0;
	output.position = mul(float4(input.position, 1.0f), WVP);
	output.uv = input.uv;
	
	return output;
}

float4 PSMain(in VSOutput input) : SV_TARGET
{
#ifdef DEFINE_TEST
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
#endif
	
	return m_atlas.Sample(m_sampler, input.uv);
}

// The default pass
pass Default {
	Define(DEFINE_TEST);
	
	SamplerState(m_sampler, PointClamped);
	VertexShader(VSMain, 4_0);
	PixelShader(PSMain, 4_0);
}