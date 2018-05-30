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
    static Array<RefPtr<Object>> m_objects;

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
    RefPtr<Method> FindMethod(const char* methodName) const;

    /**
    * \brief Finds field using given name.
    */
    RefPtr<Field> FindField(const char* fieldName) const;

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
    static RefPtr<Method> FindStaticMethod(const char* methodName);

    /// <summary>
    /// Creates instance of API class.
    /// WARNING: Only for API objects!
    /// Setting `initializeNativePtr` to false, can instantiate every non-static class.
    /// </summary>
    template <class T>
    static RefPtr<T> CreateInstance(const char* ns, const char* className, RefPtr<Assembly> assembly = nullptr,
                                 bool initializeNativePtr = true)
    {
        RefPtr<T> object(new T);

        if (assembly == nullptr)
            assembly = Assembly::API;

        auto cls = assembly->FindClass(ns, className);
        auto objectPtr = static_cast<RefPtr<Object>>(object);
        Create(objectPtr, Domain::Root->GetMono(), cls->m_class, initializeNativePtr);
        return object;
    }

    /// <summary>
    /// Creates instance of API class.
    /// WARNING: Only for API objects!
    /// Setting `initializeNativePtr` to false, can instantiate every non-static class.
    /// </summary>
    template <class T>
    static void CreateInstance(RefPtr<T> object, const char* ns, const char* className, RefPtr<Assembly> assembly = nullptr,
                               bool initializeNativePtr = true)
    {
        if (assembly == nullptr)
            assembly = Assembly::API;

        auto cls = assembly->FindClass(ns, className);
        auto objectPtr = static_cast<RefPtr<Object>>(object);
        Create(objectPtr, Domain::Root->GetMono(), cls->m_class, initializeNativePtr);
    }

    /**
     * \brief Initializes an instance of given object.
     * \param object The object to be initialized.
     * \param instance The instance to be initialized. 
     */
    template <class T>
    static void InitializeInstance(RefPtr<T> object, MonoObject* instance)
    {
        auto objectPtr = static_cast<RefPtr<Object>>(object);
        InitializeInstance(objectPtr, instance);
    }

public:
    static void Create(RefPtr<Object>& object, MonoDomain* domain, MonoClass* monoClass, bool isObject);
    static void InitializeInstance(RefPtr<Object>& object, MonoObject* instance);
    static void RegisterObject(RefPtr<Object> object);
    static void Destroy(Object* object);
    static void DestroyAll();
    static void Finalize(Object* object);

    FORCEINLINE static void Destroy(RefPtr<Object> object)
    {
        Destroy(object.get());
    }
};

#endif // OBJECT_H
