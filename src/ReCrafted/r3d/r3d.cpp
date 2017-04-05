// r3d © 2016-2017 Damian 'Erdroy' Korczowski

#include "r3d.h"
#include "r3d_renderer.h"
#include "r3d_d3d11.h"
#include "r3d_headers.h"
#include "r3d_commandlist.h"
#include "r3d_config.h"
#include "r3d_filesystem.h"

namespace r3d
{

#define R3D_DEFINE_HANDLE_ALLOCATOR(type, name, maxval) type name##_table[maxval] = {}; \
	type alloc_##name##() { \
		for(auto i = 1; i < maxval; i++) { \
			if( name##_table[i].idx == 0u ) { \
				name##_table[i].idx = i; \
				name##_table[i].ctx = g_currentCtx; \
				return name##_table[i]; } \
		} \
		throw; \
	}  \
	void free_##name##( type value ) { _ASSERT( value.idx != 0u ); name##_table[ value.idx ].idx = 0u; }

#define R3D_CTX_OK(handle) _ASSERT(##handle##.ctx == g_currentCtx)
#define R3D_CTX_OK_PTR(handle) _ASSERT(##handle##->ctx == g_currentCtx)

	class window
	{
	public:
		void* nativeptr = nullptr;

	public:

	};

	window* g_windows[R3D_MAX_WINDOWS] = {};
	r3d_commandlist g_cmdlist = {};
	r3d_apitype::Enum g_apitype = r3d_apitype::d3d11;
	r3d_renderer* g_renderer = nullptr;
	int g_currentCtx = 0;


	// allocators
	R3D_DEFINE_HANDLE_ALLOCATOR(r3d_window_handle, window_handle, R3D_MAX_WINDOWS)
	R3D_DEFINE_HANDLE_ALLOCATOR(r3d_renderbuffer_handle, renderbuffer_handle, R3D_MAX_RENDERBUFFERS)
	R3D_DEFINE_HANDLE_ALLOCATOR(r3d_vertexlayout_handle, vertexlayout_handle, R3D_MAX_VERTEXLAYOUTS)
	R3D_DEFINE_HANDLE_ALLOCATOR(r3d_vertexbuffer_handle, vertexbuffer_handle, R3D_MAX_VERTEXBUFFERS)
	R3D_DEFINE_HANDLE_ALLOCATOR(r3d_indexbuffer_handle, indexbuffer_handle, R3D_MAX_INDEXBUFFERS)
	R3D_DEFINE_HANDLE_ALLOCATOR(r3d_texture2d_handle, texture_handle, R3D_MAX_TEXTURES)
	R3D_DEFINE_HANDLE_ALLOCATOR(r3d_shader_handle, shader_handle, R3D_MAX_SHADERS)


	r3d_commandlist alloc_commandlist(uint32_t size)
	{
		auto memory = alloc_memory(size);
		return r3d_commandlist(memory, size);
	}

	void init(r3d_apitype::Enum api_type)
	{
		g_apitype = api_type;

		// allocate command list
		g_cmdlist = alloc_commandlist(R3D_INTERNAL_DEFAULT_CMDLIST_SIZE);

		switch (api_type)
		{
		case r3d_apitype::d3d11: 
			g_renderer = new r3d_d3d11;
			g_renderer->init();
			break;
		case r3d_apitype::opengl4:
			//renderer = new r3d_opengl4;
			//renderer->init();
			break;
		case r3d_apitype::d3d12:
			//renderer = new r3d_d3d12;
			//renderer->init();
			break;
		case r3d_apitype::vulkan:
			//renderer = new r3d_vk;
			//renderer->init();
			break;
		default: throw;
		}
	}

	void beginframe()
	{
		g_cmdlist.write(r3d_cmdlist_header::beginframe);
	}

	void endframe()
	{
		g_cmdlist.write(r3d_cmdlist_header::present);

		// execute command list
		g_renderer->execute_commandlist(&g_cmdlist);

		// clear command list
		g_cmdlist.clear();
	}

	void shutdown()
	{
		// destroy command list
		g_cmdlist.destroy();

		// destroy renderer
		g_renderer->destroy();
	}

	r3d_apitype::Enum get_apitype()
	{
		return g_apitype;
	}

	r3d_memory alloc_memory(uint32_t size)
	{
		_ASSERT(size != 0);
		return static_cast<r3d_memory>(malloc(size));
	}

	void destroy_memory(r3d_memory* memory)
	{
		_ASSERT(*memory != nullptr);
		free(*memory);
	}

	r3d_window_handle create_window(void* window_handle, const char* title)
	{
		auto handle = alloc_window_handle();

		g_windows[handle.idx] = new window;

		auto window = g_windows[handle.idx];
		window->nativeptr = window_handle;

		// use title for something?

		g_renderer->create_context(&handle);
		return handle;
	}

	void window_makecurrent(r3d_window_handle* window)
	{
		g_currentCtx = window->idx;
		g_renderer->make_current(window);
	}

	void* window_getnativeptr(r3d_window_handle* window)
	{
		return g_windows[window->idx]->nativeptr;
	}

	void vsync(bool enabled)
	{
		g_cmdlist.write(r3d_cmdlist_header::vsync);
		g_cmdlist.write(enabled);
	}

	void set_output(void* window_handle)
	{
		g_cmdlist.write(r3d_cmdlist_header::target_window);
		g_cmdlist.write(window_handle);
	}

	void set_rendermode(int16_t mode)
	{
		g_cmdlist.write(r3d_cmdlist_header::rendermode);
		g_cmdlist.write(mode);
	}
	
	void shader_load(const char* shader_file, r3d_shader_handle* shader_handle)
	{
		FILE* file_stream = nullptr;
		if (r3d_filesystem::openFile(shader_file, &file_stream, true))
		{
			// TODO: for all platforms

			// temporary: read vertex shader
			char vs_head[8] = {};
			auto vs_size = 0u;
			r3d_filesystem::read(&file_stream, vs_head, 8u);
			r3d_filesystem::read(&file_stream, &vs_size, 4u);

			auto vs_data = new unsigned char[vs_size];
			r3d_filesystem::read(&file_stream, vs_data, vs_size);

			g_cmdlist.write(r3d_cmdlist_header::load_vshader);
			g_cmdlist.write(shader_handle->idx);
			g_cmdlist.write(vs_size);
			g_cmdlist.write(vs_data);

			// temporary: read pixel shader
			char ps_head[8] = {};
			auto ps_size = 0u;
			r3d_filesystem::read(&file_stream, ps_head, 8u);
			r3d_filesystem::read(&file_stream, &ps_size, 4u);

			auto ps_data = new unsigned char[ps_size];
			r3d_filesystem::read(&file_stream, ps_data, ps_size);

			g_cmdlist.write(r3d_cmdlist_header::load_pshader);
			g_cmdlist.write(shader_handle->idx);
			g_cmdlist.write(ps_size);
			g_cmdlist.write(ps_data);
		}
		r3d_filesystem::closeFile(&file_stream);
	}

	void shader_apply(r3d_shader_handle* shader_handle)
	{
		g_cmdlist.write(r3d_cmdlist_header::shader_apply);
		g_cmdlist.write(shader_handle->idx);
	}


	void use_renderbuffer(r3d_renderbuffer_handle* renderbuffer)
	{

	}

	void clear_color(float color[4])
	{
		g_cmdlist.write(r3d_cmdlist_header::clearcolor);
		g_cmdlist.write(color);
	}

	void clear_depth()
	{
		g_cmdlist.write(r3d_cmdlist_header::cleardepth);
	}

	r3d_vertexbuffer_handle create_vertexbuffer(r3d_vertexlayout_handle* vertexlayout, uint32_t verticeCount, r3d_memory* data)
	{
		auto handle = alloc_vertexbuffer_handle();

		g_cmdlist.write(r3d_cmdlist_header::create_vertexbuffer);
		g_cmdlist.write(handle.idx);
		g_cmdlist.write(vertexlayout);
		g_cmdlist.write(verticeCount);
		g_cmdlist.write(data);

		return handle;
	}

	r3d_indexbuffer_handle create_indexbuffer(uint32_t indiceCount, r3d_memory* data, bool use32bit)
	{
		auto handle = alloc_indexbuffer_handle();

		g_cmdlist.write(r3d_cmdlist_header::create_indexbuffer);
		g_cmdlist.write(handle.idx);
		g_cmdlist.write(indiceCount);
		g_cmdlist.write(data);
		g_cmdlist.write(use32bit);

		return handle;
	}

	r3d_shader_handle create_shader()
	{
		return alloc_shader_handle();
	}
}
