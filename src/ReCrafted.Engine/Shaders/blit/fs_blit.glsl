// ReCrafted (c) 2016-2018 Always Too Late

// inputs
$input m_uv

#include "../ShaderAPI.glsl"

SAMPLER2D(m_texture0, 0);

PixelShader()
{
	vec4 color = texture2D(m_texture0, m_uv);
	FS_OUTPUT(0, color);
}