// ReCrafted (c) 2016-2018 Always Too Late

// inputs
$input m_normal, m_color, m_depth

#include "../ShaderAPI.glsl"

SAMPLER2D(m_texture, 0);

PixelShader()
{
	//FS_OUTPUT(0, texture2D(m_texture, m_uv));

    FS_OUTPUT(0, m_color);
	FS_OUTPUT(1, vec4(m_normal, 0.0f));

	gl_FragDepth = m_depth;
}