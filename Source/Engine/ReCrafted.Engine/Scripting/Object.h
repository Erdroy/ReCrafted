// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "ObjectManager.h"
#include "Mono.h"
#include "Class.h"

/// <summary>
///     Object class. Base class for all mono object instances.
/// </summary>
API_CLASS(public, partial, customNamespace="ReCrafted.API")
class Object
{
    API_CLASS_BODY()
    friend class ObjectManager;

private:
    ObjectId_t m_id = 0;
    GCHandle_t m_gcHandle = 0;
    MonoObject* m_managedInstance = nullptr;
    Class m_class = Class{ nullptr };

private:
    void Initialize(GCHandle_t gcHandle, MonoObject* managedInstance, const Class& cls);

public:
    /// <summary>
    ///     Returns true when valid.
    /// </summary>
    bool IsValid() const;

    /// <summary>
    ///     Gets class of this object.
    /// </summary>
    Class GetClass() const;

    /// <summary>
    ///     Gets managed object pointer.
    /// </summary>
    MonoObject* ToManaged() const;

public:
    /// <summary>
    ///     Destroys given Object instance.
    /// </summary>
    API_FUNCTION()
    static void Destroy(Object* objectInstance);
    
    /// <summary>
    ///     Destroys given Object instance now.
    /// </summary>
    API_FUNCTION()
    static void DestroyNow(Object* objectInstance);

    template <class TObject>
    static TObject* New(const RefPtr<Assembly>& assembly = nullptr, const RefPtr<Domain>& domain = nullptr)
    {
        static_assert(std::is_same<TObject, Object>::value, "TObject must inherit from Object class.");

        const auto object = new TObject();
        ObjectManager::GetInstance()->InitializeInstance(object, TObject::Namespace(), TObject::Name(), assembly, domain);
        return object;
    }

    static Object* New(const char* nameSpace, const char* name, const RefPtr<Assembly>& assembly, const RefPtr<Domain>& domain = nullptr)
    {
        const auto object = new Object();
        ObjectManager::GetInstance()->InitializeInstance(object, nameSpace, name, assembly, domain);
        return object;
    }
};
