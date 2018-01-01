// ReCrafted (c) 2016-2018 Always Too Late

#include "Method.h"
#include "Assembly.h"

void Method::invoke(void** params) const
{
    mono_runtime_invoke(m_method, m_object, params, nullptr);
}

void Method::invokeStatic(void** params) const
{
    mono_runtime_invoke(m_method, nullptr, params, nullptr);
}
