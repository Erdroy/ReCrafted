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
		opengl4,
		d3d12,
		vulkan
	};
};

struct r3d_object_handle
{
public:
	unsigned int idx = 0u;
};

#pragma warning(push)
#pragma warning(disable : 4003)

#define R3D_DEFINE_HANDLE(name, addit)	struct r3d_##name##_handle : public r3d_object_handle { addit };
#define R3D_FORCEINLINE					__forceinline
#define R3D_INLINE						inline


#define R3D_RENDERMODE_NONE				UINT16_C(0x00)
#define R3D_RENDERMODE_DEFAULT			UINT16_C(0x01)
#define R3D_RENDERMODE_WIREFRAME		UINT16_C(0x02)


R3D_DEFINE_HANDLE(vertexbuffer)
R3D_DEFINE_HANDLE(indexbuffer)
R3D_DEFINE_HANDLE(texture2d)
R3D_DEFINE_HANDLE(texture3d)
R3D_DEFINE_HANDLE(shader)

#pragma warning(pop)

namespace r3d
{

	// core functions

	void init(void* window_handle, r3d_apitype::Enum api_type = r3d_apitype::d3d11);
	void frame();
	void shutdown();

	R3D_FORCEINLINE r3d_memory alloc_memory(uint32_t size);
	R3D_FORCEINLINE void destroy_memory(r3d_memory* memory);


	// rendering options

	R3D_FORCEINLINE void vsync(bool enabled);
	R3D_FORCEINLINE void set_output(void* window_handle);
	R3D_FORCEINLINE void set_rendermode(int16_t mode);

	// rendering methods

	void load_shader(const char* shader_bytecode, r3d_shader_handle* shader_handle);

	R3D_FORCEINLINE void clear_color(float color[4]);
	R3D_FORCEINLINE void clear_depth();

	R3D_FORCEINLINE r3d_vertexbuffer_handle* create_vertexbuffer();
	R3D_FORCEINLINE r3d_indexbuffer_handle* create_indexbuffer(uint32_t size, bool use32bit);
	R3D_FORCEINLINE r3d_texture2d_handle* create_texture2d(uint16_t width, uint16_t height, uint16_t format);
	R3D_FORCEINLINE r3d_texture3d_handle* create_renderbuffer(r3d_texture2d_handle* texture_handles, uint8_t texture_count);
	R3D_FORCEINLINE r3d_shader_handle* create_shader();

	R3D_FORCEINLINE void destroy_vertexbuffer(r3d_vertexbuffer_handle* handle);
	R3D_FORCEINLINE void destroy_indexbuffer(r3d_indexbuffer_handle* handle);
	R3D_FORCEINLINE void destroy_texture2d(r3d_texture2d_handle* handle);
	R3D_FORCEINLINE void destroy_texture3d(r3d_texture3d_handle* handle);
	R3D_FORCEINLINE void destroy_shader(r3d_shader_handle* handle);
}

#endif // R3D_H
