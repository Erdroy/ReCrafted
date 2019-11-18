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

        for (auto i = 0u; i < threadCount; i++)
        {
            dataBegin[i] = 0;
            commandCount[i] = 0;
        }

        dataBegin[0] = 0;
        dataEnd[0] = size;
        commandCount[0] = m_commandCount;
    }

    void CommandList::Destroy()
    {
        // free the memory
        delete[] static_cast<byte*>(cmdlist);
        cmdlist = nullptr;
    }
}
