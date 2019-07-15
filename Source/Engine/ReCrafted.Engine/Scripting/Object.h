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
    uint32_t m_refCount = 1u;
    MonoObject* m_managedInstance = nullptr;
    Class m_class = Class{ nullptr };

public:
    Object() = default;
    virtual ~Object() = default;

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

    /// <summary>
    ///     Sets native pointer of this object.
    /// </summary>
    /// <remarks>When managed object does not derive from Object, this call will be ignored.</remarks>
    void SetNativePointer(void* pointer);

public:
    /// <summary>
    ///     Adds single reference count to the object's refCount variable.
    /// </summary>
    API_FUNCTION(internal);
    void AddRef();

    /// <summary>
    ///     Removes single reference from the object's refCount variable and returns true when it's refCount is 0.
    /// </summary>
    API_FUNCTION(internal);
    bool RemoveRef();

    /// <summary>
    ///     Sets the object's refCount variable to 1.
    /// </summary>
    API_FUNCTION(internal);
    void ResetRefCount();

    /// <summary>
    ///     Returns the ref count of this object.
    /// </summary>
    /// <returns>This object's reference count.</returns>
    API_FUNCTION(internal);
    uint32_t GetRefCount() const
    {
        return m_refCount;
    }

public:
    /// <summary>
    /// Creates new object from template type TObject.
    /// </summary>
    template <class TObject>
    static TObject* New(const RefPtr<Assembly>& assembly = nullptr, const RefPtr<Domain>& domain = nullptr)
    {
        static_assert(std::is_base_of<Object, TObject>::value, "TObject must inherit from Object class.");

        const auto object = new TObject();
        ObjectManager::GetInstance()->InitializeInstance(object, TObject::Namespace(), TObject::Name(), assembly, domain);
        return object;
    }

    /// <summary>
    /// Creates new object from specified namespace and name.
    /// </summary>
    static Object* New(const char* nameSpace, const char* name, const RefPtr<Assembly>& assembly, const RefPtr<Domain>& domain = nullptr)
    {
        const auto object = new Object();
        ObjectManager::GetInstance()->InitializeInstance(object, nameSpace, name, assembly, domain);
        return object;
    }

public:
    /// <summary>
    ///     Destroys given Object instance.
    /// </summary>
    API_FUNCTION(multithread);
    static void Destroy(Object* objectInstance);
    
    /// <summary>
    ///     Destroys given Object instance now.
    /// </summary>
    API_FUNCTION(multithread);
    static void DestroyNow(Object* objectInstance);

private:
    /// <summary>
    ///     Creates new object based on mono type.
    /// </summary>
    /// <param name="type">The mono type.</param>
    /// <returns>The managed object pointer.</returns>
    /// <remarks>Mono type must derive from Object.</remarks>
    API_FUNCTION(noproxy, multithread);
    static MonoObject* New(MonoType* type);

    /// <summary>
    ///     Creates new generic object based on mono type.
    /// </summary>
    /// <param name="baseType">The mono base type.</param>
    /// <param name="type">The mono type.</param>
    /// <param name="obj">The base object type.</param>
    /// <returns>The managed object pointer.</returns>
    /// <remarks>Mono type must derive from Object.</remarks>
    API_FUNCTION(noproxy, multithread);
    static MonoObject* NewGeneric(MonoType* baseType, MonoType* type, MonoObject* obj);
};
