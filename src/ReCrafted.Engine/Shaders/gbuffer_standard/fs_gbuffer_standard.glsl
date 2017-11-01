// ReCrafted © 2016-2017 Always Too Late

// inputs
$input m_uv, m_normal, m_color

#include "../ShaderAPI.glsl"

SAMPLER2D(m_texture, 0);

PixelShader()
{
	FS_OUTPUT(0, texture2D(m_texture, m_uv));
	//           ALBEDO

	FS_OUTPUT(1, vec4(m_normal, m_color.r));
	//                 NORMAL    AO
}