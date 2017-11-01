// ReCrafted © 2016-2017 Always Too Late

#include "Method.h"
#include "Assembly.h"

void Method::invoke() const
{
    mono_runtime_invoke(m_method, m_object, nullptr, nullptr);
}
