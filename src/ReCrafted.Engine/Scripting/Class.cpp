// ReCrafted (c) 2016-2019 Always Too Late

#include "Class.h"

char* Class::GetType() const
{
    return mono_type_get_name(mono_class_get_type(m_class));
}
