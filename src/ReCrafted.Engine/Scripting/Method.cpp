// ReCrafted (c) 2016-2018 Always Too Late

#include "Method.h"
#include "Assembly.h"

void Method::Invoke(void** params) const
{
    MonoObject* exception = nullptr;
    mono_runtime_invoke(m_method, m_object, params, &exception);

    // TODO: Raise exception
}

void Method::InvokeStatic(void** params) const
{
    MonoObject* exception = nullptr;
    mono_runtime_invoke(m_method, nullptr, params, &exception);

    // TODO: Raise exception
}
