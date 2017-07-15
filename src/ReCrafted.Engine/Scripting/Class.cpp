// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Class.h"
#include "Domain.h"
#include "Object.h"

Ptr<Object> Class::createInstance(bool isObject) const
{
	Ptr<Object> object(new Object);

	auto instance = mono_object_new(m_assembly->m_domain, m_class);
	mono_runtime_object_init(instance);

	if (isObject)
	{
		// TODO: call constructor

		/*void *args[1];
		args[0] = object.get();
		mono_runtime_invoke(ctor_method, instance, args, nullptr);*/
	}

	object->m_object = instance;
	object->m_class = m_class;

	return object;
}
