// ReCrafted (c) 2016-2019 Always Too Late

#include "Field.h"

void Field::SetValue(void* value) const
{
    mono_field_set_value(m_object, m_field, value);
}

void Field::GetValue(void* value) const
{
    mono_field_get_value(m_object, m_field, value);
}

FieldType::_enum Field::GetType() const
{
    return static_cast<FieldType::_enum>(mono_type_get_type(mono_field_get_type(m_field)));
}
