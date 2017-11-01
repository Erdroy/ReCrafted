// ReCrafted © 2016-2017 Always Too Late

#include "Class.h"

char* Class::getType() const
{
	return  mono_type_get_name(mono_class_get_type(m_class));
}
