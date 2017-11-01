// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef OBJECT_H
#define OBJECT_H

// includes
#include "Common/ReCraftedAPI.h"
#include "Mono.h"
#include "Assembly.h"
#include "Class.h"
#include "Domain.h"
#include "Core/Types.h"

class Method;
class Field;

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
	/// <summary>
	/// Creates instance of API class.
	/// WARNING: Only for API objects!
	/// Setting `initializeNativePtr` to false, can instantiate every non-static class.
	/// </summary>
	template <class T>
	static Ptr<T> createInstance(const char* ns, const char* className, Ptr<Assembly> assembly = nullptr, bool initializeNativePtr = true)
	{
		Ptr<T> object(new T);

		if (assembly == nullptr)
			assembly = Assembly::API;

		auto cls = assembly->findClass(ns, className);
		create(static_cast<Ptr<Object>>(object), Domain::Root->getMono(), cls->m_class, initializeNativePtr);
		return object;
	}

    /// <summary>
    /// Creates instance of API class.
    /// WARNING: Only for API objects!
    /// Setting `initializeNativePtr` to false, can instantiate every non-static class.
    /// </summary>
    template <class T>
    static void createInstance(Ptr<T> object, const char* ns, const char* className, Ptr<Assembly> assembly = nullptr, bool initializeNativePtr = true)
    {
        if (assembly == nullptr)
            assembly = Assembly::API;

        auto cls = assembly->findClass(ns, className);
        create(static_cast<Ptr<Object>>(object), Domain::Root->getMono(), cls->m_class, initializeNativePtr);
    }

public:
	static void create(Ptr<Object>& object, MonoDomain* domain, MonoClass* monoClass, bool isObject);
	static void registerObject(Ptr<Object> object);
	static void destroy(Object* object);
	static void destroyall();
	static void finalize(Object* object);

};

#endif // OBJECT_H
