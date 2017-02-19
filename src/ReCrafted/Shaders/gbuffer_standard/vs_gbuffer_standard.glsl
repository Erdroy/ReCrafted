// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

// inputs, outputs
$input position, uv, normal
$output m_uv, m_normal

#include "../ShaderAPI.glsl"

VertexShader()
{
	gl_Position = mul(vec4(position, 1.0), m_modelViewProjection);
	m_normal = normalize(float4(normal, 1.0f)) * 0.5f + 0.5f;
	m_uv = uv;
}
