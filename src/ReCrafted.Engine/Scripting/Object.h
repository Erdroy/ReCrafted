// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef OBJECT_H
#define OBJECT_H

// includes
#include "Common/ReCraftedAPI.h"
#include "Method.h"
#include "Field.h"
#include "Mono.h"
#include "Utils/Types.h"

#include <vector>

class Object
{
	friend class Class;
	API_DEF

private:
	static std::vector<Ptr<Object>> m_objects;

private:
	MonoObject* m_object = nullptr;
	MonoClass* m_class = nullptr;
	uint32_t m_gchandle = 0u;

public:
	/// <summary>
	/// Finds method using given name.
	/// </summary>
	Ptr<Method> findMethod(const char* methodName) const;

	/// <summary>
	/// Finds field using given name.
	/// </summary>
	Ptr<Field> findField(const char* fieldName) const;

	/// <summary>
	/// Gets managed object pointer.
	/// </summary>
	MonoObject* getManagedPtr() const;

public:
	static void create(Ptr<Object>& object, MonoDomain* domain, MonoClass* monoClass, bool isObject);
	static void registerObject(Ptr<Object> object);
	static void destroy(Object* object);
	static void destroyall();
	static void finalize(Object* object);
};

#endif // OBJECT_H
