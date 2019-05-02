// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Field.h"
#include "Object.h"

void Field::SetValue(Object* instance, void* value) const
{
    ASSERT(m_field);

    mono_field_set_value(instance->ToManaged(), m_field, value);
}

void Field::GetValue(Object* instance, void* value) const
{
    ASSERT(m_field);

    mono_field_get_value(instance->ToManaged(), m_field, value);
}

FieldType Field::GetType() const
{
    ASSERT(m_field);

    return static_cast<FieldType>(mono_type_get_type(mono_field_get_type(m_field)));
}

bool Field::IsValid() const
{
    return m_field;
}

MonoClassField* Field::ToMono() const
{
    return m_field;
}
