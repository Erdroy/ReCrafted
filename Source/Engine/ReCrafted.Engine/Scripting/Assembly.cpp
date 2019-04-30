// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Assembly.h"

void Assembly::Unload()
{
    ASSERT(m_assembly);
    // mono_assembly_close(m_assembly); // BUG: This causes the `mono_jit_cleanup` to crash. Overall this is not really required to call.
    m_assembly = nullptr;
}
