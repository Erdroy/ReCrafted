// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Class.h"
#include "Domain.h"
#include "Object.h"

Ptr<Object> Class::createInstance(bool isObject) const
{
	Ptr<Object> object(new Object);

	auto instance = mono_object_new(m_assembly->m_domain, m_class);
	mono_runtime_object_init(instance);

	object->m_object = instance;
	object->m_class = m_class;

	if (isObject)
	{
		// TODO: set native pointer

	}

	return object;
}
