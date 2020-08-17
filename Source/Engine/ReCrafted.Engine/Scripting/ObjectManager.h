// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/Lock.h"
#include "Common/Action.h"
#include "Core/SubSystems/SubSystem.h"
#include "Scripting/Mono.h"

#include <sparsepp/spp.h>
#include <concurrentqueue.h>

using ObjectId_t = uint32_t;
using GCHandle_t = uint32_t;

class ObjectManager final : public SubSystem<ObjectManager>
{
    using ObjectMap_t = spp::sparse_hash_map<GCHandle_t, Object*>;

private:
    std::atomic<ObjectId_t> m_lastObjectId = 0;
    std::atomic<GCHandle_t> m_objectCount = 0;
    ObjectMap_t m_objectMap;
    Lock m_objectMapLock;

    spp::sparse_hash_map<uint32_t, Action<Object*, bool>> m_objectCreators;

    moodycamel::ConcurrentQueue<Object*> m_destroyQueue;

private:
    void RegisterObject(Object* object);

    void UnregisterObject(Object* object);
    void ReleaseObject(Object* object);
    void DeleteObject(Object* object);

    /// <summary>
    ///     Shorthand for fully releasing object using:
    ///     UnregisterObject(object)
    ///     ReleaseObject(object)
    ///     DeleteObject(object)
    /// </summary>
    /// <param name="object">The object to destroy.</param>
    void DestroyObject(Object* object);

    void ReleaseQueuedObjects();

protected:
    void Initialize() override;
    void Shutdown() override;

    void OnLateUpdate() override;

public:
    /// <summary>
    ///     Registers object creator delegate for given mono type.
    /// </summary>
    /// <param name="type">The mono type.</param>
    /// <param name="creator">The object creator.</param>
    void RegisterObjectCreator(MonoType* type, const Action<Object*, bool>& creator);

    /// <summary>
    ///     Gets object creator delegate for given mono type.
    /// </summary>
    /// <param name="type">The mono type.</param>
    /// <remarks>The object creator.</remarks>
    Action<Object*, bool>* GetObjectCreator(MonoType* type);

    /// <summary>
    ///     Initializes managed instance of given object.
    /// </summary>
    /// <param name="object">The object instance.</param>
    /// <param name="nameSpace">The namespace.</param>
    /// <param name="name">The class name.</param>
    /// <param name="assembly">The assembly.</param>
    /// <param name="domain">The domain.</param>
    void InitializeInstance(Object* object, const char* nameSpace, const char* name, const RefPtr<Assembly>& assembly, const RefPtr<Domain>& domain);

    /// <summary>
    ///     Initializes managed instance of given object.
    /// </summary>
    /// <param name="object">The object instance.</param>
    /// <param name="managedObject">The managed object instance.</param>
    /// <param name="managedObjectClass">The managed object class.</param>
    void InitializeInstance(Object* object, MonoObject* managedObject, MonoClass* managedObjectClass);

public:
    /// <summary>
    ///     Destroys given Object instance.
    /// </summary>
    static void Destroy(Object* objectInstance);
    
    /// <summary>
    ///     Destroys given Object instance now.
    /// </summary>
    static void DestroyNow(Object* objectInstance);

    /// <summary>
    ///     Creates new object based on mono type.
    /// </summary>
    /// <param name="type">The mono type.</param>
    /// <returns>The managed object pointer.</returns>
    /// <remarks>Mono type must derive from Object.</remarks>
    static MonoObject* New(MonoType* type);

    /// <summary>
    ///     Creates new generic object based on mono type.
    /// </summary>
    /// <param name="baseType">The mono base type.</param>
    /// <param name="type">The mono type.</param>
    /// <param name="obj">The base object type.</param>
    /// <returns>The managed object pointer.</returns>
    /// <remarks>Mono type must derive from Object.</remarks>
    static MonoObject* NewGeneric(MonoType* baseType, MonoType* type, MonoObject* obj);

    /// <summary>
    ///     Attaches current thread to the Mono backend.
    ///     Allows the C++ engine to call Mono functions.
    /// </summary>
    static void AttachThread();

    /// <summary>
    ///     Detaches current thread from the Mono backend.
    ///     Calling Mono functions from C++ engine is no longer valid.
    /// </summary>
    static void DetachThread();
};
