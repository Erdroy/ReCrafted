// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Object.h"

#include "Core/Logger.h"
#include "Graphics/Camera.h"
#include "Method.h"
#include "Field.h"

#include <algorithm>

std::vector<Ptr<Object>> Object::m_objects;

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

MonoObject* Object::getManagedPtr() const
{
	return m_object;
}

void Object::create(Ptr<Object>& object, MonoDomain* domain, MonoClass* monoClass, bool isObject)
{
	auto instance = mono_object_new(domain, monoClass);
	mono_runtime_object_init(instance);

	// get garbage collector handle, and mark it pinned
	auto gch = mono_gchandle_new(instance, true);

	object->m_gchandle = gch;
	object->m_object = instance;
	object->m_class = monoClass;

	if (isObject)
	{
		// set native pointer
		auto testField = object->findField("NativePtr");
		testField->setValue(&object);

		// register object
		registerObject(object);
	}
}

void Object::registerObject(Ptr<Object> object)
{
	m_objects.push_back(object);
}

void Object::destroy(Object* object)
{
	// free garbage collector handle
	mono_gchandle_free(object->m_gchandle);
	object->m_gchandle = 0u;

	// unregister
	for (std::vector<Ptr<Object>>::iterator iter = m_objects.begin(); iter != m_objects.end(); ++iter)
	{
		if (iter->get() == object)
		{
			m_objects.erase(iter);
			break;
		}
	}
}

void Object::destroyall()
{
	for(auto i = 0u; i < m_objects.size(); i ++)
	{
		auto object = m_objects[i];

		// free garbage collector handle
		mono_gchandle_free(object->m_gchandle);
	}

	m_objects.clear();
}

void Object::finalize(Object* object)
{
	// unregister
	for (std::vector<Ptr<Object>>::iterator iter = m_objects.begin(); iter != m_objects.end(); ++iter)
	{
		if (iter->get() == object)
		{
			Logger::write("Object was finalized, but not destroyed at first!", LogLevel::Warning);
			m_objects.erase(iter);
			break;
		}
	}

	// cleanup
	object->m_object = nullptr;
}
