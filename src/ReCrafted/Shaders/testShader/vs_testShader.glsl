// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

// inputs, outputs
$input position
$output color

#include "../ShaderAPI.glsl"

VertexShader()
{
	gl_Position = vec4(position, 1.0);
	color = vec4(1, 1, 1, 1);
}
