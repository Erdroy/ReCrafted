// ReCrafted © 2016-2017 Always Too Late

// inputs, outputs
$input position, color
$output m_color

#include "../ShaderAPI.glsl"

VertexShader()
{
	gl_Position = mul(vec4(position, 1.0), m_modelViewProjection);
	m_color = color;
}
