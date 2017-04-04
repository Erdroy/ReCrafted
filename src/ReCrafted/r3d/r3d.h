// r3d © 2016-2017 Damian 'Erdroy' Korczowski

#ifndef R3D_H
#define R3D_H

// includes
#include <cstdint>
#include "r3d_types.h"

struct r3d_apitype
{
	enum Enum
	{
		d3d11,
		d3d12,
		opengl4,
		vulkan
	};
};

struct r3d_shadertype
{
	enum Enum
	{
		unknown,
		vertexshader,
		pixelshader,
		computeshader,

		count
	};
};

struct r3d_texture_format
{
	enum Enum
	{
		unknown,

		// color formats
		r1,
		a8,
		r8,
		r8i,
		r8u,
		r8s,
		r16,
		r16i,
		r16u,
		r16f,
		r16s,
		r32i,
		r32u,
		r32f,
		rg8,
		rg8i,
		rg8u,
		rg8s,
		rg16,
		rg16i,
		rg16u,
		rg16f,
		rg16s,
		rg32i,
		rg32u,
		rg32f,
		rgb8,
		rgb8i,
		rgb8u,
		rgb8s,
		rgb9e5f,
		bgra8,
		rgba8,
		rgba8i,
		rgba8u,
		rgba8s,
		rgba16,
		rgba16i,
		rgba16u,
		rgba16f,
		rgba16s,
		rgba32i,
		rgba32u,
		rgba32f,
		r5g6b5,
		rgba4,
		rgb5a1,
		rgb10a2,
		r11g11b10f,

		// depth formats
		d16,
		d16f,
		d24,
		d24f,
		d24s8,
		d32,
		d32f,

		count,
	};
};

struct r3d_object_handle
{
public:
	unsigned int idx = 0u;
	unsigned int ctx = 0u;
};

#pragma warning(push)
#pragma warning(disable : 4003)

#define R3D_DEFINE_HANDLE(name, addit)	struct r3d_##name##_handle : public r3d_object_handle { addit };
#define R3D_FORCEINLINE					__forceinline
#define R3D_INLINE						inline


#define R3D_RENDERMODE_NONE				UINT16_C(0x00)
#define R3D_RENDERMODE_DEFAULT			UINT16_C(0x01)
#define R3D_RENDERMODE_WIREFRAME		UINT16_C(0x02)



R3D_DEFINE_HANDLE(window)
R3D_DEFINE_HANDLE(renderbuffer)
R3D_DEFINE_HANDLE(vertexlayout)
R3D_DEFINE_HANDLE(vertexbuffer)
R3D_DEFINE_HANDLE(indexbuffer)
R3D_DEFINE_HANDLE(texture2d)
R3D_DEFINE_HANDLE(texture3d)
R3D_DEFINE_HANDLE(shader)

#pragma warning(pop)

namespace r3d
{

	// core functions

	void init(r3d_apitype::Enum api_type = r3d_apitype::d3d11);
	void beginframe();
	void endframe();
	void shutdown();
	r3d_apitype::Enum get_apitype();


	// memory management functions

	r3d_memory alloc_memory(uint32_t size);
	void destroy_memory(r3d_memory* memory);


	// window(context) functions

	r3d_window_handle create_window(const char* title); // TODO: create_window
	r3d_window_handle create_window(void* window_handle, const char* title);

	void window_makecurrent(r3d_window_handle* window);
	void* window_getnativeptr(r3d_window_handle* window);

	void destroy_window(r3d_window_handle* window); // TODO: destroy_window


	// rendering options

	void vsync(bool enabled);
	void set_output(void* window_handle);
	void set_rendermode(int16_t mode);


	// shader methods

	void compile_shader(const char* shader_file, const char* output_file, bool all_platforms = true);
	void load_shader(const char* shader_file, r3d_shader_handle* shader_handle);


	// rendering methods

	void use_renderbuffer(r3d_renderbuffer_handle* renderbuffer); // TODO: use_renderbuffer

	void clear_color(float color[4]);
	void clear_depth();

	r3d_renderbuffer_handle create_renderbuffer(uint16_t texture_count, r3d_texture2d_handle* textures[]); // TODO: create_renderbuffer
	r3d_vertexbuffer_handle create_vertexbuffer(r3d_vertexlayout_handle* vertexlayout, uint32_t verticeCount, r3d_memory* data);
	r3d_indexbuffer_handle create_indexbuffer(uint32_t indiceCount, r3d_memory* data, bool use32bit = false);
	r3d_texture2d_handle create_texture2d(uint16_t width, uint16_t height, r3d_texture_format format); // TODO: create_renderbuffer
	r3d_shader_handle create_shader();

	void destroy_renderbuffer(r3d_renderbuffer_handle* handle); // TODO: destroy_renderbuffer
	void destroy_vertexbuffer(r3d_vertexbuffer_handle* handle); // TODO: destroy_vertexbuffer
	void destroy_indexbuffer(r3d_indexbuffer_handle* handle); // TODO: destroy_indexbuffer
	void destroy_texture2d(r3d_texture2d_handle* handle); // TODO: destroy_texture2d
	void destroy_texture3d(r3d_texture3d_handle* handle); // TODO: destroy_texture3d
	void destroy_shader(r3d_shader_handle* handle); // TODO: destroy_shader
}

#endif // R3D_H
