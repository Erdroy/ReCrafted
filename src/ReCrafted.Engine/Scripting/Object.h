// ReCrafted (c) 2016-2018 Always Too Late

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
#include "Core/Containers/Array.h"

#include <vector>

class Method;
class Field;

class Object
{
    friend class Class;

private:
	SCRIPTING_API_IMPL()

private:
	static Array<Ref<Object>> m_objects;

private:
	MonoObject* m_object = nullptr;
	MonoClass* m_class = nullptr;
	uint32_t m_gchandle = 0u;

public:
    virtual ~Object() = default;

public:
    /**
    * \brief Finds method using given name.
    */
	Ref<Method> findMethod(const char* methodName) const;

    /**
    * \brief Finds field using given name.
    */
	Ref<Field> findField(const char* fieldName) const;

    /**
    * \brief Gets managed object pointer.
    */
	MonoObject* getManagedPtr() const;

    /**
     * \brief Called when object is being destroyed.
     */
    virtual void onDestroy() {}

public:
    static Ref<Method> findStaticMethod(const char* methodName);

	/// <summary>
	/// Creates instance of API class.
	/// WARNING: Only for API objects!
	/// Setting `initializeNativePtr` to false, can instantiate every non-static class.
	/// </summary>
	template <class T>
	static Ref<T> createInstance(const char* ns, const char* className, Ref<Assembly> assembly = nullptr, bool initializeNativePtr = true)
	{
		Ref<T> object(new T);

		if (assembly == nullptr)
			assembly = Assembly::API;

		auto cls = assembly->findClass(ns, className);
        auto objectPtr = static_cast<Ref<Object>>(object);
	    create(objectPtr, Domain::Root->getMono(), cls->m_class, initializeNativePtr);
		return object;
	}

    /// <summary>
    /// Creates instance of API class.
    /// WARNING: Only for API objects!
    /// Setting `initializeNativePtr` to false, can instantiate every non-static class.
    /// </summary>
    template <class T>
    static void createInstance(Ref<T> object, const char* ns, const char* className, Ref<Assembly> assembly = nullptr, bool initializeNativePtr = true)
    {
        if (assembly == nullptr)
            assembly = Assembly::API;

        auto cls = assembly->findClass(ns, className);
        auto objectPtr = static_cast<Ref<Object>>(object);
        create(objectPtr, Domain::Root->getMono(), cls->m_class, initializeNativePtr);
    }

    /**
     * \brief Initializes an instance of given object.
     * \param object The object to be initialized.
     * \param instance The instance to be initialized. 
     */
    template <class T>
    static void initializeInstance(Ref<T> object, MonoObject* instance)
	{
        auto objectPtr = static_cast<Ref<Object>>(object);
        initializeInstance(objectPtr, instance);
	}

public:
	static void create(Ref<Object>& object, MonoDomain* domain, MonoClass* monoClass, bool isObject);
    static void initializeInstance(Ref<Object>& object, MonoObject* instance);
	static void registerObject(Ref<Object> object);
	static void destroy(Object* object);
	static void destroyall();
	static void finalize(Object* object);

    FORCEINLINE static void destroy(Ref<Object> object)
    {
        destroy(object.get());
    }

};

#endif // OBJECT_H
