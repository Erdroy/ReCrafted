// r3d © 2016-2017 Damian 'Erdroy' Korczowski

#include "r3d.h"
#include "r3d_renderer.h"
#include "r3d_d3d11.h"
#include "r3d_headers.h"
#include "r3d_commandlist.h"

namespace r3d
{
#define MAX_WINDOWS 32

	class window
	{
	public:
		void* nativeptr = nullptr;

	public:

	};

	window* g_windows[MAX_WINDOWS] = {};
	r3d_commandlist g_cmdlist = {};
	r3d_apitype::Enum g_apitype = r3d_apitype::d3d11;
	r3d_renderer* g_renderer = nullptr;


	r3d_commandlist alloc_commandlist(uint32_t size)
	{
		auto memory = alloc_memory(size);
		return r3d_commandlist(memory, size);
	}

	void init(r3d_apitype::Enum api_type)
	{
		g_apitype = api_type;

		// allocate command list
		g_cmdlist = alloc_commandlist(64 << 10);

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

	void vsync(bool enabled)
	{
		// write init
		g_cmdlist.write(r3d_cmdlist_header::vsync);
		g_cmdlist.write(enabled);

	}

	void set_output(void* window_handle)
	{
		// write target window handle
		g_cmdlist.write(r3d_cmdlist_header::target_window);
		g_cmdlist.write(window_handle);
	}

	void set_rendermode(int16_t mode)
	{
		g_cmdlist.write(r3d_cmdlist_header::rendermode);
		g_cmdlist.write(mode);
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

	void viewport(int x, int y, int width, int height)
	{
		g_cmdlist.write(r3d_cmdlist_header::viewport);
		g_cmdlist.write(x);
		g_cmdlist.write(y);
		g_cmdlist.write(width);
		g_cmdlist.write(height);
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

	void window_makecurrent(r3d_window_handle* window)
	{
		g_renderer->make_current(window);
	}

	void* window_getnativeptr(r3d_window_handle* window)
	{
		return g_windows[window->idx]->nativeptr;
	}

	r3d_window_handle create_window(const char* title)
	{
		auto handle = r3d_window_handle();
		handle.idx = -1;

		for (auto i = 0; i < MAX_WINDOWS; i++)
		{
			if (g_windows[i] == nullptr)
			{
				handle.idx = i;
				break;
			}
		}

		if (handle.idx == -1)
			throw;

		g_windows[handle.idx] = new window;
		auto window = g_windows[handle.idx];

		// TODO: create window etc.
		// TODO: window events

		g_renderer->create_context(&handle);
		return handle;
	}

	r3d_window_handle create_window(void* window_handle, const char* title)
	{
		auto handle = r3d_window_handle();
		handle.idx = -1;

		for (auto i = 0; i < MAX_WINDOWS; i++)
		{
			if (g_windows[i] == nullptr)
			{
				handle.idx = i;
				break;
			}
		}

		if (handle.idx == -1)
			throw;

		g_windows[handle.idx] = new window;

		auto window = g_windows[handle.idx];
		window->nativeptr = window_handle;

		// use title for something?


		g_renderer->create_context(&handle);
		return handle;
	}

	void destroy_window(r3d_window_handle* window)
	{
		// TODO: destroy window and context(this must happen on the end of rendering, do not release context when there is some instructions for the same context).
	}
}
