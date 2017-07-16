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
		// create object
		Ptr<T> object(new T);
		Object::create(static_cast<Ptr<Object>>(object), Domain::Root->getMono(), m_class, isObject);
		return object;
	}

	/// <summary>
	/// Gets mono type.
	/// </summary>
	int getType() const
	{
		return mono_type_get_type(mono_class_get_type(m_class));
	}
};

#endif // CLASS_H
