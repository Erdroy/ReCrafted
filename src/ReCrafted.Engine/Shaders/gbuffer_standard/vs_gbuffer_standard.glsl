// ReCrafted (c) 2016-2018 Always Too Late

// inputs, outputs
$input position, normal, color
$output m_normal, m_color, m_depth

#include "../ShaderAPI.glsl"

VertexShader()
{
	gl_Position = mul(vec4(position, 1.0), m_modelViewProjection);
	m_normal = normalize(normal) * 0.5f + 0.5f;
	//m_uv = uv;
	m_color = color;

    const float far = 10000.0f;
	const float C = 0.01f;
	const float FC = 1.0f / log(far * C + 1.0);
	m_depth = log(gl_Position.w * C + 1.0) * FC;

	//float posz = (2.0 * m_depth - 1.0) * gl_Position.w;
    //gl_Position.z = posz;
}
