// r3d © 2016-2017 Damian 'Erdroy' Korczowski

#ifndef R3D_COMMANDLIST_H
#define R3D_COMMANDLIST_H

// includes
#include "r3d.h"
#include <cstdlib>
#include <crtdbg.h>
#include <corecrt_memcpy_s.h>

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

	DEF_WRITTER(void*) // pointer
	DEF_WRITTER(float)
	DEF_WRITTER(bool)
	DEF_WRITTER(r3d_byte)
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


#endif // R3D_COMMANDLIST_H
