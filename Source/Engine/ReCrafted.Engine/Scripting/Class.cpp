// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Class.h"
#include "Field.h"
#include "Method.h"

Class Class::GetBaseClass() const
{
    const auto base = mono_class_get_parent(m_class);

    if (!base)
        return Class(nullptr);

    return Class(base);
}

Method Class::GetMethod(const char* methodName) const
{
    ASSERT(m_class);

    const auto methodDescription = mono_method_desc_new(methodName, true);

    if (!methodDescription)
        return Method(nullptr);

    const auto methodHandle = mono_method_desc_search_in_class(methodDescription, m_class);
    mono_method_desc_free(methodDescription);

    if (!methodHandle)
        return Method(nullptr);

    return Method(methodHandle);
}

Field Class::GetField(const char* fieldName) const
{
    ASSERT(m_class);

    const auto field = mono_class_get_field_from_name(m_class, fieldName);

    if (!field)
        return Field(nullptr);

    return Field(field);
}

bool Class::IsValid() const
{
    return m_class;
}

char* Class::Name() const
{
    ASSERT(m_class);
    return mono_type_get_name(mono_class_get_type(m_class));
}

MonoClass* Class::ToMono() const
{
    return m_class;
}
