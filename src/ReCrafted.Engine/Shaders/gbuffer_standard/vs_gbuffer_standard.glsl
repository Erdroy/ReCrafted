// ReCrafted © 2016-2017 Always Too Late

// inputs, outputs
$input position, normal, color
$output m_normal, m_color

#include "../ShaderAPI.glsl"

VertexShader()
{
	gl_Position = mul(vec4(position, 1.0), m_modelViewProjection);
	m_normal = normalize(normal) * 0.5f + 0.5f;
	//m_uv = uv;
	m_color = color;
}
