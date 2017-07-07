// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

// inputs
$input m_uv, m_color

#include "../ShaderAPI.glsl"

SAMPLER2D(m_texture0, 0);

PixelShader()
{
	if (m_uv[1] > 0)
	{
		vec4 color = texture2D(m_texture0, m_uv);

		FS_OUTPUT(0, color * m_color);
		return;
	}

	FS_OUTPUT(0, m_color);
}