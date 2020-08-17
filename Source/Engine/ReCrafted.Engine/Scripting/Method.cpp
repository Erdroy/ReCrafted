// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Method.h"
#include "Object.h"

void Method::Invoke(Object* instance, void** params) const
{
    ASSERT(m_method);

    MonoObject* exception = nullptr;
    mono_runtime_invoke(m_method, instance ? instance->ToManaged() : nullptr, params, &exception);

    if(exception)
        mono_unhandled_exception(exception);
}

void Method::InvokeStatic(void** params) const
{
    Invoke(nullptr, params);
}

bool Method::IsValid() const
{
    return m_method;
}

MonoMethod* Method::ToMono() const
{
    return m_method;
}
