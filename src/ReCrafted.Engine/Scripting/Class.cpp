// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Class.h"

char* Class::getType() const
{
	return  mono_type_get_name(mono_class_get_type(m_class));
}
