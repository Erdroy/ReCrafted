// ReCrafted © 2016-2017 Always Too Late

// inputs, outputs
$input position, uv, color
$output m_uv, m_color

#include "../ShaderAPI.glsl"

VertexShader()
{
	gl_Position = vec4(position, 1.0);
	m_uv = uv;
	m_color = color;
}
