// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

// inputs
$input m_uv

#include "../ShaderAPI.glsl"

SAMPLER2D(m_texture0, 0); // albedo
SAMPLER2D(m_texture1, 1); // normals, ambient occulusion
//SAMPLER2D(m_texture2, 2); // shadow occulusion

vec3 m_lightdir;

PixelShader()
{
	vec4 albedo = texture2D(m_texture0, m_uv);
	vec4 norm_ao = texture2D(m_texture1, m_uv);
	//float shadows = texture2D(m_texture2, m_uv);

	// decode normal
	vec3 normal = norm_ao.rgb * 2.0f - 1.0f;

	// decode ao
	float ao = norm_ao.a;






	FS_OUTPUT(0, albedo);
}