// ReCrafted © 2016 Damian 'Erdroy' Korczowski

struct VSOutput
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

Texture2D m_texture : register(t0);
SamplerState m_sampler : register(s0);

VSOutput VSMain(uint vert : SV_VERTEXID)
{
	VSOutput output = (VSOutput)0;

	float2 texcoord = float2(vert & 1, vert >> 1);
	float4 position = float4((texcoord.x - 0.5f) * 2, -(texcoord.y - 0.5f) * 2, 0.0f, 1.0f);

	output.position = position;
	output.uv = texcoord;

	return output;
}

float4 PSMain(in VSOutput input) : SV_TARGET
{
	return m_texture.Sample(m_sampler, input.uv);
}