// ReCrafted (c) 2016-2018 Always Too Late

#include "CommandList.h"

namespace GFXL
{
	void CommandList::Initialize(uint32_t size)
	{
		_ASSERT(size != 0);
		
		cmdlist = static_cast<GFXLMemory>(new byte[size]);
		this->size = size;
	}

	void CommandList::Assign(uint8_t threadCount, uint32_t* dataBegin, uint32_t* commandCount)
	{
		_ASSERT(threadCount);
		_ASSERT(dataBegin);
		_ASSERT(commandCount);

		auto commandsPerThread = m_commandCount / threadCount;
		auto command = 0u;
		auto thread = 0u;

		auto position = 0u;

		// Fix division problem (3 / 4 = 0)
		if(m_commandCount < threadCount)
		{
			commandsPerThread = 1;
			threadCount = m_commandCount;
		}

		 if (commandsPerThread == 0)
			return;

		dataBegin[thread] = 0;

		for(auto i = 0u; i < m_commandCount; i ++)
		{
			auto spos = position;
			Read_GFXLCommandHeader(&position);
			auto size = Read_uint16(&position);
			position = spos + size;
			command++;

			// Read till command count is >= commandsPerThread - write command count,
			// if we filled all allowed commands, increment thread, write data start and continue

			if(command >= commandsPerThread)
			{
				commandCount[thread] = command;

				if(thread + 1 >= threadCount)
				{
					_ASSERT(i < m_commandCount);
					break;
				}

				thread++;

				dataBegin[thread] = position;
				command = 0u;
			}
		}
	}

	void CommandList::Destroy()
	{
		// free the memory
		delete[] cmdlist;
		cmdlist = nullptr;
	}
}
