// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Assembly.h"

Ptr<Class> Assembly::findClass(const char* class_namespace, const char* class_name)
{
	Ptr<Class> newClass(new Class);
	newClass->m_class = mono_class_from_name(m_image, class_namespace, class_name);
	newClass->m_assembly = this;
	return newClass;
}
