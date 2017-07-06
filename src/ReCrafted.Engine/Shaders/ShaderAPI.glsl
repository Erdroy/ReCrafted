// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

// include BGFX shader api as the base for all the shaders
#include "../../../bgfx_shader.glsl"

// shader methods
#define PixelShader void main
#define VertexShader void main

// uniforms
uniform mat4 m_modelViewProjection;

// defines
#define FS_OUTPUT(pos, data) gl_FragData[pos] = data