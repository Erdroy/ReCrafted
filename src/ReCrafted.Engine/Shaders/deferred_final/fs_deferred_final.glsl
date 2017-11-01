// ReCrafted © 2016-2017 Always Too Late

// inputs
$input m_uv

#include "../ShaderAPI.glsl"

SAMPLER2D(m_texture0, 0); // albedo
SAMPLER2D(m_texture1, 1); // normals, ambient occulusion
//SAMPLER2D(m_texture2, 2); // shadow occulusion

vec3 m_lightdir;
vec4 m_lightcolor;

PixelShader()
{
	vec4 albedo = texture2D(m_texture0, m_uv);
	vec4 norm_ao = texture2D(m_texture1, m_uv);
	//float shadow = texture2D(m_texture2, m_uv).r;

	// decode normal
	vec3 normal = norm_ao.rgb * 2.0f - 1.0f;

	// calculate ao
	float ao = 1.0f - norm_ao.a;

	vec3 lightDir = normalize(m_lightdir);
	float nDotL = dot(normal, lightDir);
	nDotL = clamp(nDotL, 0.0f, 1.0f);

	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 lighting = (lightColor * nDotL)/* * shadow */+ vec3(0.6f, 0.6f, 0.6f);

	FS_OUTPUT(0, vec4(albedo.rgb * lighting * ao, 1.0f));
}