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
#define DEF_WRITER(type) R3D_FORCEINLINE void write( type header) { write(&header, sizeof( type )); }
#define DEF_READER(type, name) R3D_FORCEINLINE type read_##name() { type tmp; read(&tmp, sizeof( type )); return tmp; }

public:
	r3d_commandlist() {}
	r3d_commandlist(r3d_memory memory, uint32_t size) : size(size), cmdlist(memory) {}

public:
	R3D_FORCEINLINE void read(void* data, uint32_t size)
	{
		_ASSERT(rposition < this->size);
		_ASSERT(data != nullptr);

		auto ptr = this->cmdlist + rposition;
		memcpy_s(data, size, ptr, size);
		rposition += size;
	}

	R3D_FORCEINLINE void write(void* data, uint32_t size)
	{
		if (wposition + size >= this->size)
			resize(this->size + (8 << 10)); // resize up by adding 8k bytes

		auto ptr = this->cmdlist + wposition;
		memcpy_s(ptr, this->size, data, size);
		wposition += size;
	}

	// readers
	DEF_READER(void*, ptr) // pointer
	DEF_READER(float, float)
	DEF_READER(bool, bool)	  
	DEF_READER(r3d_byte, byte)
	DEF_READER(uint16_t, uint16)
	DEF_READER(uint32_t, uint32)
	DEF_READER(uint64_t, uint64)
	DEF_READER(int16_t, int16)
	DEF_READER(int32_t, int32)
	DEF_READER(int64_t, int64)

	// writers
	DEF_WRITER(void*) // pointer
	DEF_WRITER(float)
	DEF_WRITER(bool)
	DEF_WRITER(r3d_byte)
	DEF_WRITER(uint16_t)
	DEF_WRITER(uint32_t)
	DEF_WRITER(uint64_t)
	DEF_WRITER(int16_t)
	DEF_WRITER(int32_t)
	DEF_WRITER(int64_t)

	R3D_FORCEINLINE bool is_read_end() const
	{
		return rposition >= size || rposition >= wposition;
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

	void clear()
	{
		_ASSERT(size > 0);

		memset(cmdlist, 0, size);

		rposition = 0u;
		wposition = 0u;
	}

	void destroy()
	{
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
