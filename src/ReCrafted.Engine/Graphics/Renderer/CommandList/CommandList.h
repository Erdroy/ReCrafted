// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef COMMANDLIST_H
#define COMMANDLIST_H

#include "../Renderer.hpp"
#include "../RendererDefines.h"
#include "Commands.h"

namespace Renderer
{
#define DefineWriter(type) RENDERER_FORCEINLINE void Write( type header) { Write(&header, sizeof( type )); }
#define DefineReader(type, name) RENDERER_FORCEINLINE type Read_##name(uint32_t* position) { type tmp; Read(&tmp, position, sizeof( type )); return tmp; }
#define DefineReaderWriter(type, name)	\
			DefineWriter(type);			\
			DefineReader(type, name);

	class CommandList
	{
	private:
		uint32_t m_commandCount = 0u;

	public:
		uint32_t size = 0u;
		uint32_t wposition = 0u;
		RendererMemory cmdlist = nullptr;

	public:
		void Initialize(uint32_t size = RENDERER_COMMAND_LIST_SIZE);

		void Assign(uint8_t threadCount, uint32_t* dataBegin, uint32_t* dataEnd, uint32_t* commandCount);

		void Destroy();

	public:
		RENDERER_FORCEINLINE void Read(void* data, uint32_t* position, uint32_t size) const
		{
			_ASSERT(*position < this->size);
			_ASSERT(data != nullptr);

			auto ptr = cmdlist + *position;
			memcpy_s(data, size, ptr, size);
			*position += size;
		}

		RENDERER_FORCEINLINE void Write(void* data, uint32_t size)
		{
			if (wposition + size >= this->size)
				Resize(this->size + RENDERER_COMMAND_LIST_INCREMENT); // resize up by adding 8k bytes

			auto ptr = cmdlist + wposition;
			memcpy_s(ptr, this->size, data, size);

			wposition += size;
		}

		template<typename T>
		RENDERER_FORCEINLINE void WriteCommand(T* command);

		template <typename T>
		T ReadCommand(uint32_t* position);

	public:
		void Resize(uint32_t size)
		{
            // we don't really want to resize down or when it is 0, lol
            if (this->size <= size || size == 0)
                return;

			auto new_cmdlist = static_cast<byte*>(realloc(cmdlist, size));

			// check if the realloc actually reallocated the memory
			_ASSERT(new_cmdlist != nullptr);

			// everything is ok, so now, just set the command list as the new
			this->size = size;
			cmdlist = new_cmdlist;
		}

		void Clear()
		{
			_ASSERT(size > 0);

#if RENDERER_COMMAND_LIST_FULL_DISCARD
			memset(cmdlist, 0, size);
#endif
			m_commandCount = 0u;
			wposition = 0u;
		}

	public:
		// ReSharper disable CppRedundantEmptyDeclaration

		DefineReaderWriter(CommandHeader::_enum, CommandHeader);
		DefineReaderWriter(RendererMemory, RendererMemory);
		DefineReaderWriter(ObjectHandle, Handle);
		DefineReaderWriter(bool, bool);
		DefineReaderWriter(byte, byte);
		DefineReaderWriter(float, float);
		DefineReaderWriter(uint16_t, uint16);
		DefineReaderWriter(uint32_t, uint32);
		DefineReaderWriter(uint64_t, uint64);
		DefineReaderWriter(int16_t, int16);
		DefineReaderWriter(int32_t, int32);
		DefineReaderWriter(int64_t, int64);

		// ReSharper restore CppRedundantEmptyDeclaration
	};

	template <typename T>
	void CommandList::WriteCommand(T* command)
	{
		Write(command, sizeof(T));
		m_commandCount++;
	}

	template <typename T>
	T CommandList::ReadCommand(uint32_t* position)
	{
		T cmd = {};

		Read((void*)&cmd, position, sizeof T);

		return cmd;
	}
}

#endif // COMMANDLIST_H
