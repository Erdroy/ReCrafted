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
    static Array<Object*> m_objects;

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

    /**
     * \brief Creates instance of API class.
     * \note Only for API objects!
     * Setting `initializeNativePtr` to false, can instantiate every non-static class.
     */
    template <class TType>
    static TType* CreateAssetInstance(const char* ns, const char* className, RefPtr<Assembly> assembly = nullptr,
        bool initializeNativePtr = true)
    {
        cvar object = ContentManager::CreateVirtualAsset<TType>();

        if (assembly == nullptr)
            assembly = Assembly::API;

        auto cls = assembly->FindClass(ns, className);
        Create(object, Domain::Root->GetMono(), cls->m_class, initializeNativePtr);
        return object;
    }
    
    /**
     * \brief Creates instance of API class.
     * \note Only for API objects! 
     * Setting `initializeNativePtr` to false, can instantiate every non-static class.
     */
    template <class T>
    static T* CreateInstance(const char* ns, const char* className, RefPtr<Assembly> assembly = nullptr,
                                 bool initializeNativePtr = true)
    {
        cvar object = new T();

        if (assembly == nullptr)
            assembly = Assembly::API;

        auto cls = assembly->FindClass(ns, className);
        Create(object, Domain::Root->GetMono(), cls->m_class, initializeNativePtr);
        return object;
    }

    /**
     * \brief Creates instance of API class.
     * \note Only for API objects! 
     * Setting `initializeNativePtr` to false, can instantiate every non-static class.
     */
    template <class T>
    static void CreateInstance(T* object, const char* ns, const char* className, RefPtr<Assembly> assembly = nullptr,
                               bool initializeNativePtr = true)
    {
        if (assembly == nullptr)
            assembly = Assembly::API;

        auto cls = assembly->FindClass(ns, className);
        Create(object, Domain::Root->GetMono(), cls->m_class, initializeNativePtr);
    }

    /**
     * \brief Initializes an instance of given object.
     * \param object The object to be initialized.
     * \param instance The instance to be initialized.
     */
    template <class T>
    static void InitializeInstanceType(T* object, MonoObject* instance)
    {
        InitializeInstance(object, instance);
    }

public:
    static bool IsObjectInitialized(Object* object);
    static MonoObject* Create(Object* object, MonoDomain* domain, MonoClass* monoClass, bool isObject);
    static void InitializeInstance(Object* object, MonoObject* instance);
    static void RegisterObject(Object* object);
    static void Destroy(Object* object);
    static void Release(Object* object);
    static void UnbindManaged(Object* object);
    static void DestroyAll();
    static void Finalize(Object* object);
};

#endif // OBJECT_H
