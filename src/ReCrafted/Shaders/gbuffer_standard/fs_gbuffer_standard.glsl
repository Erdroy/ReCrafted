// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

// inputs
$input m_uv, m_normal

#include "../ShaderAPI.glsl"

PixelShader()
{
	FS_OUTPUT(0, vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//                        ALBEDO

	FS_OUTPUT(1, vec4(m_normal, 1.0f));
	//                 NORMAL    AO
}