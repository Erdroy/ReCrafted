// r3d © 2016-2017 Damian 'Erdroy' Korczowski

#ifndef R3D_H
#define R3D_H

// includes
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <crtdbg.h>


// r3d defines

#define R3D_DEFINE_HANDLE(name, addit) struct r3d_##name##_handle : public r3d_object_handle { addit };
#define R3D_FORCEINLINE __forceinline
#define R3D_INLINE inline


// r3d types

typedef unsigned char r3d_byte;
typedef r3d_byte* r3d_memory;


// r3d objects

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

R3D_DEFINE_HANDLE(vertexbuffer)
R3D_DEFINE_HANDLE(indexbuffer)
R3D_DEFINE_HANDLE(texture2d)
R3D_DEFINE_HANDLE(texture3d)
R3D_DEFINE_HANDLE(shader)

#pragma warning(pop)

struct r3d_cmdlist_header
{
	enum Enum
	{
		unknown = 0,

		vsync,
		viewport,

		count
	};
};

class r3d_commandlist
{
#define DEF_WRITTER(type) void write( type header) { write(&header, sizeof( type )); }

public:
	r3d_commandlist() {}
	r3d_commandlist(r3d_memory memory, uint32_t size) : size(size), cmdlist(memory) {}

public:
	void read(void* data, uint32_t size)
	{
		_ASSERT(rposition < size);
		_ASSERT(data != nullptr);

		auto ptr = this->cmdlist + rposition;
		memcpy_s(data, size, ptr, this->size);
		rposition += size;
	}

	void write(void* data, uint32_t size)
	{
		if (wposition + size >= this->size)
			resize(this->size + (8 << 10)); // resize up by adding 8k bytes

		auto ptr = this->cmdlist + wposition;
		memcpy_s(ptr, this->size, data, size);
		rposition += size;
	}

	void write(r3d_cmdlist_header::Enum header)
	{
		auto data = static_cast<r3d_byte>(header);
		write(&data, 1u);
	}

	DEF_WRITTER(void*) // poiter
	DEF_WRITTER(bool)
	DEF_WRITTER(uint16_t)
	DEF_WRITTER(uint32_t)
	DEF_WRITTER(uint64_t)
	DEF_WRITTER(int16_t)
	DEF_WRITTER(int32_t)
	DEF_WRITTER(int64_t)

	bool is_read_end() const
	{
		return rposition >= size;
	}

public:
	void resize(uint32_t size)
	{
		auto new_cmdlist = static_cast<r3d_byte*>(realloc(cmdlist, size));

		// check if the realloc actually reallocated the memory
		_ASSERT(new_cmdlist != nullptr);

		// everything is ok, so now, just set the command list as the new
		this->size = size;
		cmdlist = new_cmdlist;
	}

	R3D_FORCEINLINE void clear()
	{
		_ASSERT(new_cmdlist != nullptr);
		_ASSERT(size > 0);

		memset(cmdlist, 0, size);

		rposition = 0u;
		wposition = 0u;
	}

	R3D_FORCEINLINE void destroy()
	{
		_ASSERT(new_cmdlist != nullptr);

		// free the memory
		free(static_cast<void*>(cmdlist));
		cmdlist = nullptr;
	}

public:
	uint32_t size = 0u;
	uint32_t wposition = 0u;
	uint32_t rposition = 0u;
	r3d_memory cmdlist = nullptr;
};

namespace r3d
{

	// core functions

	void init(void* windowhandle, r3d_apitype::Enum api_type = r3d_apitype::d3d11);
	void frame();
	void shutdown();

	R3D_FORCEINLINE r3d_memory alloc_memory(uint32_t size);
	R3D_FORCEINLINE void destroy_memory(r3d_memory* memory);


	// command list management methods

	r3d_commandlist alloc_commandlist(uint32_t size);


	// rendering options

	R3D_FORCEINLINE void vsync(bool enabled);


	// rendering methods

	void load_shader(const char* shader_bytecode, r3d_shader_handle* shader_handle);

	R3D_FORCEINLINE r3d_vertexbuffer_handle* create_vertexbuffer();
	R3D_FORCEINLINE r3d_indexbuffer_handle* create_indexbuffer();
	R3D_FORCEINLINE r3d_texture2d_handle* create_texture2d();
	R3D_FORCEINLINE r3d_texture3d_handle* create_renderbuffer();
	R3D_FORCEINLINE r3d_shader_handle* create_shader();

	R3D_FORCEINLINE void destroy_vertexbuffer(r3d_vertexbuffer_handle* handle);
	R3D_FORCEINLINE void destroy_indexbuffer(r3d_indexbuffer_handle* handle);
	R3D_FORCEINLINE void destroy_texture2d(r3d_texture2d_handle* handle);
	R3D_FORCEINLINE void destroy_texture3d(r3d_texture3d_handle* handle);
	R3D_FORCEINLINE void destroy_shader(r3d_shader_handle* handle);
}

#endif // R3D_H
