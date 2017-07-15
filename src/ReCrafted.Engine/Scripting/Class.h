// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef CLASS_H
#define CLASS_H

// includes
#include "Utils/Types.h"
#include "Object.h"
#include "Mono.h"

class Class
{
	friend class ScriptingEngine;
	friend class Assembly;

private:
	Assembly* m_assembly = nullptr;
	MonoClass* m_class = nullptr;

public:
	/// <summary>
	/// Creates instance of this class.
	/// </summary>
	template <class T>
	Ptr<T> createInstance(bool isObject = true)
	{
		Ptr<T> object(new T);
		
		auto instance = mono_object_new(m_assembly->m_domain, m_class);
		mono_runtime_object_init(instance);

		// get garbage collector handle, and mark it pinned
		auto gch = mono_gchandle_new(instance, true);

		object->m_gchandle = gch;
		object->m_object = instance;
		object->m_class = m_class;

		if (isObject)
		{
			// set native pointer
			auto testField = object->findField("NativePtr");

			auto ptr = object.get();
			testField->setValue(&ptr);
		}

		return object;
	}
};

#endif // CLASS_H
