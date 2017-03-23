// r3d © 2016-2017 Damian 'Erdroy' Korczowski

#include "r3d.h"
#include "r3d_renderer.h"
#include "r3d_d3d11.h"

namespace r3d
{
	r3d_commandlist g_cmdlist = {};
	r3d_renderer* renderer = nullptr;

	void init(void* windowhandle, r3d_apitype::Enum api_type)
	{
		// allocate command list
		g_cmdlist = alloc_commandlist(64 << 10);

		switch (api_type)
		{
		case r3d_apitype::d3d11: 
			renderer = new r3d_d3d11;
			break;
		case r3d_apitype::opengl4: 
			throw;
		case r3d_apitype::d3d12: 
			throw;
		case r3d_apitype::vulkan: 
			throw;
		default: throw;
		}
	}

	void vsync(bool enabled)
	{
		// write init
		g_cmdlist.write(r3d_cmdlist_header::vsync);
		g_cmdlist.write(enabled);

	}

	void viewport(int x, int y, int width, int height)
	{
		g_cmdlist.write(r3d_cmdlist_header::viewport);
		g_cmdlist.write(x);
		g_cmdlist.write(y);
		g_cmdlist.write(width);
		g_cmdlist.write(height);
	}

	void frame()
	{
		// execute command list
		renderer->execute_commandlist(&g_cmdlist);

		// clear command list
		g_cmdlist.clear();
	}

	void shutdown()
	{
		// destroy command list
		g_cmdlist.destroy();

		// destroy renderer
		renderer->destroy();
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

	r3d_commandlist alloc_commandlist(uint32_t size)
	{
		auto memory = alloc_memory(size);
		return r3d_commandlist(memory, size);
	}
}
