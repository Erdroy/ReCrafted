// ReCrafted (c) 2016-2018 Always Too Late

// inputs
$input m_color

#include "../ShaderAPI.glsl"

SAMPLER2D(m_texture0, 0);

PixelShader()
{
	// output
	FS_OUTPUT(0, m_color);
}