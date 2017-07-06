// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

// inputs, outputs
$input position, uv
$output m_uv

#include "../ShaderAPI.glsl"

VertexShader()
{
	gl_Position = vec4(position, 1.0);
	m_uv = uv;
}
