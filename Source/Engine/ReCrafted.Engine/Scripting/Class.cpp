// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Class.h"

char* Class::Name() const
{
    ASSERT(m_class);
    return mono_type_get_name(mono_class_get_type(m_class));
}

MonoClass* Class::ToMono() const
{
    return m_class;
}
