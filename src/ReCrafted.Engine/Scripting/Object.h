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
    Ref<Method> FindMethod(const char* methodName) const;

    /**
    * \brief Finds field using given name.
    */
    Ref<Field> FindField(const char* fieldName) const;

    /**
    * \brief Gets managed object pointer.
    */
    MonoObject* GetManagedPtr() const;

    /**
     * \brief Called when object is being destroyed.
     */
    virtual void OnDestroy()
    {
    }

public:
    static Ref<Method> FindStaticMethod(const char* methodName);

    /// <summary>
    /// Creates instance of API class.
    /// WARNING: Only for API objects!
    /// Setting `initializeNativePtr` to false, can instantiate every non-static class.
    /// </summary>
    template <class T>
    static Ref<T> CreateInstance(const char* ns, const char* className, Ref<Assembly> assembly = nullptr,
                                 bool initializeNativePtr = true)
    {
        Ref<T> object(new T);

        if (assembly == nullptr)
            assembly = Assembly::API;

        auto cls = assembly->FindClass(ns, className);
        auto objectPtr = static_cast<Ref<Object>>(object);
        Create(objectPtr, Domain::Root->GetMono(), cls->m_class, initializeNativePtr);
        return object;
    }

    /// <summary>
    /// Creates instance of API class.
    /// WARNING: Only for API objects!
    /// Setting `initializeNativePtr` to false, can instantiate every non-static class.
    /// </summary>
    template <class T>
    static void CreateInstance(Ref<T> object, const char* ns, const char* className, Ref<Assembly> assembly = nullptr,
                               bool initializeNativePtr = true)
    {
        if (assembly == nullptr)
            assembly = Assembly::API;

        auto cls = assembly->FindClass(ns, className);
        auto objectPtr = static_cast<Ref<Object>>(object);
        Create(objectPtr, Domain::Root->GetMono(), cls->m_class, initializeNativePtr);
    }

    /**
     * \brief Initializes an instance of given object.
     * \param object The object to be initialized.
     * \param instance The instance to be initialized. 
     */
    template <class T>
    static void InitializeInstance(Ref<T> object, MonoObject* instance)
    {
        auto objectPtr = static_cast<Ref<Object>>(object);
        InitializeInstance(objectPtr, instance);
    }

public:
    static void Create(Ref<Object>& object, MonoDomain* domain, MonoClass* monoClass, bool isObject);
    static void InitializeInstance(Ref<Object>& object, MonoObject* instance);
    static void RegisterObject(Ref<Object> object);
    static void Destroy(Object* object);
    static void DestroyAll();
    static void Finalize(Object* object);

    FORCEINLINE static void Destroy(Ref<Object> object)
    {
        Destroy(object.get());
    }
};

#endif // OBJECT_H
