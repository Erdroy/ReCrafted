// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

// include BGFX shader api as the base for all the shaders
#include "../../../bgfx_shader.glsl"

#define PixelShader void main
#define VertexShader void main

#define FS_OUTPUT(pos, data) gl_FragData[pos] = data