// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

// inputs, outputs
$input position

#include "../ShaderAPI.glsl"

VertexShader()
{
	gl_Position = mul(vec4(position, 1.0), m_modelViewProjection);

}
