// ReCrafted © 2016-2017 Always Too Late

// inputs
$input m_uv, m_color

#include "../ShaderAPI.glsl"

SAMPLER2D(m_texture0, 0);

PixelShader()
{
	if (m_uv.x >= 0)
	{
		// sample texture
		vec4 color = texture2D(m_texture0, m_uv);

		// output
		FS_OUTPUT(0, color);
		return;
	}

	// output
	FS_OUTPUT(0, m_color);
}