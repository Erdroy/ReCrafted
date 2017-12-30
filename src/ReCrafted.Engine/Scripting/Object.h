// ReCrafted © 2016-2017 Always Too Late

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
	API_DEF

    friend class Class;

private:
	static Array<Ptr<Object>> m_objects;

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
	Ptr<Method> findMethod(const char* methodName) const;

    /**
    * \brief Finds field using given name.
    */
	Ptr<Field> findField(const char* fieldName) const;

    /**
    * \brief Gets managed object pointer.
    */
	MonoObject* getManagedPtr() const;

    /**
     * \brief Called when object is being destroyed.
     */
    virtual void onDestroy() {}

public:
    static Ptr<Method> findStaticMethod(const char* methodName);

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
        auto objectPtr = static_cast<Ptr<Object>>(object);
	    create(objectPtr, Domain::Root->getMono(), cls->m_class, initializeNativePtr);
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
        auto objectPtr = static_cast<Ptr<Object>>(object);
        create(objectPtr, Domain::Root->getMono(), cls->m_class, initializeNativePtr);
    }

    /**
     * \brief Initializes an instance of given object.
     * \param object The object to be initialized.
     * \param instance The instance to be initialized. 
     */
    template <class T>
    static void initializeInstance(Ptr<T> object, MonoObject* instance)
	{
        auto objectPtr = static_cast<Ptr<Object>>(object);
        initializeInstance(objectPtr, instance);
	}

public:
	static void create(Ptr<Object>& object, MonoDomain* domain, MonoClass* monoClass, bool isObject);
    static void initializeInstance(Ptr<Object>& object, MonoObject* instance);
	static void registerObject(Ptr<Object> object);
	static void destroy(Object* object);
	static void destroyall();
	static void finalize(Object* object);

    FORCEINLINE static void destroy(Ptr<Object> object)
    {
        destroy(object.get());
    }

};

#endif // OBJECT_H
