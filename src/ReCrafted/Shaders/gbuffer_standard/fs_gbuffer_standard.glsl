// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

// inputs
$input m_uv, m_normal, m_color

#include "../ShaderAPI.glsl"

PixelShader()
{
	FS_OUTPUT(0, vec4(0.49f, 0.3f, 0.08f, 1.0f));
	//           ALBEDO

	FS_OUTPUT(1, vec4(m_normal, m_color.r));
	//                 NORMAL    AO
}