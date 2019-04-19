// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "CommandList.h"

namespace Renderer
{
    void CommandList::Initialize(uint32_t size)
    {
        ASSERT(size != 0);

        cmdlist = static_cast<RendererMemory>(new byte[size]);
        this->size = size;
    }

    void CommandList::Assign(uint8_t threadCount, uint32_t* dataBegin, uint32_t* dataEnd, uint32_t* commandCount)
    {
        ASSERT(threadCount);
        ASSERT(dataBegin);
        ASSERT(commandCount);

        //if(m_commandCount < RENDERER_COMMAND_LIST_MIN_CMD_COUNT || threadCount == 1)
        {
            for (auto i = 0u; i < threadCount; i++)
            {
                dataBegin[i] = 0;
                commandCount[i] = 0;
            }

            dataBegin[0] = 0;
            dataEnd[0] = size;
            commandCount[0] = m_commandCount;
            //return;
        }

        /*const auto commandsPerThread = m_commandCount / threadCount;
        var commands = 0u;
        var totalSize = 0u;
        var thread = 0u;

        var position = 0u;

        dataBegin[thread] = 0;

        for(var i = 0u; i < m_commandCount; i ++)
        {
            auto spos = position;
            Read_CommandHeader(&position);
            auto size = Read_uint16(&position);
            position = spos + size;
            commands++;
            totalSize += size;

            // Read till command count is >= commandsPerThread - write command count,
            // if we filled all allowed commands, increment thread, write data start and continue

            if(commands >= commandsPerThread)
            {
                commandCount[thread] = commands;

                if(thread + 1 >= threadCount)
                {
                    ASSERT(i < m_commandCount);
                    break;
                }

                thread++;

                dataBegin[thread] = position;
                dataEnd[thread] = position + totalSize;
                commands = 0u;
                totalSize = 0;
            }
        }*/
    }

    void CommandList::Destroy()
    {
        // free the memory
        delete[] static_cast<byte*>(cmdlist);
        cmdlist = nullptr;
    }
}
