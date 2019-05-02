// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Assembly.h"
#include "Class.h"

void Assembly::Unload()
{
    ASSERT(m_assembly);
    // mono_assembly_close(m_assembly); // BUG: This causes the `mono_jit_cleanup` to crash. Overall this is not really required to call.
    m_assembly = nullptr;
}

Class Assembly::GetClass(const char* classNamespace, const char* className) const
{
    return Class(mono_class_from_name(m_image, classNamespace, className));
}

MonoAssembly* Assembly::ToMono() const
{
    return m_assembly;
}
