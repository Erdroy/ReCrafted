// ReCrafted (c) 2016-2019 Always Too Late. All rights reserved.. All rights reserved.

#include "Memory.h"

void Memory::Initialize(const AllocatorType allocatorType)
{
    switch (allocatorType)
    {
    case AllocatorType::OS:
        break;
    case AllocatorType::JEMalloc:
        break;
    case AllocatorType::TBB:
        break;
    }
}