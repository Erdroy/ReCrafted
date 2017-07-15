// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Object.h"

void Object::onFinalize()
{
	// cleanup
	m_object = nullptr;

	delete this;
}

Ptr<Method> Object::findMethod(const char* methodName) const
{
	auto methodDesc = mono_method_desc_new(methodName, true);

	if (!methodDesc)
		return nullptr;

	Ptr<Method> method(new Method);
	method->m_object = m_object;
	method->m_method = mono_method_desc_search_in_class(methodDesc, m_class);
	return method;
}

Ptr<Field> Object::findField(const char* fieldName) const
{
	auto fieldDesc = mono_class_get_field_from_name(m_class, fieldName);

	if (!fieldDesc)
		return nullptr;

	Ptr<Field> field(new Field);

	field->m_object = m_object;
	field->m_field = fieldDesc;

	return field;
}
