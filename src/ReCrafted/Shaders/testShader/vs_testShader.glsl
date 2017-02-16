// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

// inputs, outputs
$input position

#include "../ShaderAPI.glsl"

uniform mat4 WVP;

VertexShader()
{
	gl_Position = mul(vec4(position, 1.0), WVP);
	//gl_Position = mul(u_modelViewProj, vec4(position, 1.0));
	//gl_Position = vec4(position, 1.0);
}
