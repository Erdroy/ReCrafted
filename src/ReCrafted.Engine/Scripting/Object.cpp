// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Object.h"
#include <mono/metadata/debug-helpers.h>

void Object::onFinalize()
{
	// cleanup
	m_object = nullptr;
}

Ptr<Method> Object::findMethod(const char* methodName) const
{
	auto initmethoddesc = mono_method_desc_new(methodName, true);
	auto methodDesc = mono_method_desc_search_in_class(initmethoddesc, m_class);

	Ptr<Method> method(new Method);
	method->m_object = m_object;
	method->m_method = methodDesc;
	return method;
}
