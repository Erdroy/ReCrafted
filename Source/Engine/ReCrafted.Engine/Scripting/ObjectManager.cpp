// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "ObjectManager.h"
#include "Common/Logger.h"
#include "Mono.h"
#include "Assembly.h"
#include "Class.h"
#include "Domain.h"
#include "Object.h"
#include "ScriptingManager.h"
#include "Profiler/Profiler.h"

void ObjectManager::RegisterObject(Object* object)
{
    ASSERT(object);

    ScopeLock(m_objectMapLock);

    // Setup object id
    const auto objectId = m_lastObjectId++;
    object->m_id = objectId;

    // Add object to object map
    m_objectMap.insert(std::make_pair(objectId, object));

    // Increment object count
    ++m_objectCount;
}

void ObjectManager::UnregisterObject(Object* object)
{
    ASSERT(object);

    ScopeLock(m_objectMapLock);

    // Unregister
    const auto objectIterator = m_objectMap.find(object->m_id);

    if (objectIterator == m_objectMap.end())
    {
        Logger::LogWarning("Destroying object which is already destroyed (or invalid)! Pointer: {0}", reinterpret_cast<uint64_t>(object));
        m_objectMapLock.UnlockNow();
        return;
    }

    // Remove object from map
    m_objectMap.erase(objectIterator);
}

void ObjectManager::ReleaseObject(Object* object)
{
    ASSERT(object);
    ASSERT(object->m_managedInstance);

    // Reset native pointer
    // This is needed for the Shutdown cleanup, as all objects will get it's gc handles released
    // and this, all of them will finalize and we don't want them to double destroy, look: ~Object() in Object.cs.
    object->SetNativePointer(nullptr);

    // Release gc handle
    mono_gchandle_free(object->m_gcHandle);

    // Reset garbage collector handle
    object->m_gcHandle = 0u;
}

void ObjectManager::DeleteObject(Object* object)
{
    ASSERT(object);

    // Reset object's managed data
    object->m_managedInstance = nullptr;
    object->m_class = Class{ nullptr };

    // Reset object id
    object->m_id = 0u;

    // Decrement object count
    --m_objectCount;

    delete object;
}

void ObjectManager::DestroyObject(Object* object)
{
    ReleaseObject(object);
    UnregisterObject(object);
    DeleteObject(object);
}

void ObjectManager::ReleaseQueuedObjects()
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    Object* toDestroy;
    while (m_destroyQueue.try_dequeue(toDestroy))
    {
        ASSERT(toDestroy);
        ASSERT(toDestroy->IsValid());

        DestroyObject(toDestroy);
    }
}

void ObjectManager::Initialize()
{
}

void ObjectManager::Shutdown()
{
    //ReleaseQueuedObjects();

    // Finalize
    Domain::Root->Finalize();

    List<Object*> objectList = {};
    m_objectMapLock.LockNow();
    {
        // Takeout all objects from object map
        // Note: This is needed, because some of the objects, 
        // do call DestroyNow function inside it's destructor calls, and this does releases the objects from the objectMap
        // and it would deadlock and create other nasty stuff like access violations, because some object was already released - 
        // and the current iterator is invalid.
        for (auto&& pair : m_objectMap)
        {
            const auto object = pair.second;

            ASSERT(object);
            ASSERT(object->IsValid());

            objectList.Add(object);
        }
    }
    m_objectMapLock.UnlockNow();

    // Destroy all objects
    for(auto&& object : objectList)
    {
        // Release and delete 
        // Note: no need to unregister, because this list will be cleared anyways, 
        // and we don't have to play with the double lock which gives deadlock and needs some workarounds)

        ReleaseObject(object);
        DeleteObject(object);
    }

    m_objectMapLock.LockNow();
    {
        m_objectMap.clear();
        m_objectCreators.clear();
    }
    m_objectMapLock.UnlockNow();

    // Make sure that we have destroyed all objects
    DEBUG_ASSERT(m_objectCount == 0);

    // Note: We don't have to release objects queued for destroy, 
    // all objects has been already destroyed, by destroying all of them from the object map.
}

void ObjectManager::OnLateUpdate()
{
    ReleaseQueuedObjects();
}

void ObjectManager::RegisterObjectCreator(MonoType* type, const Action<Object*, bool>& creator)
{
    const auto token = mono_type_get_token(type);
    m_objectCreators.insert(std::make_pair(token, creator));
}

Action<Object*, bool>* ObjectManager::GetObjectCreator(MonoType* type)
{
    const auto token = mono_type_get_token(type);

    const auto it = m_objectCreators.find(token);

    if (it == m_objectCreators.end())
        return nullptr;

    return &it->second;
}

void ObjectManager::InitializeInstance(Object* object, const char* nameSpace, const char* name, const RefPtr<Assembly>& assembly, const RefPtr<Domain>& domain)
{
    const auto targetAssembly = assembly ? assembly : ScriptingManager::GetAPIAssembly();

    ASSERT(object);
    ASSERT(targetAssembly);

    const auto cls = targetAssembly->GetClass(nameSpace, name);

    _ASSERT_(cls.IsValid(), "Could not find class type. Make sure that the namespace and class name does exist in the given assembly.");

    const auto managedInstance = mono_object_new(domain ? domain->ToMono() : Domain::Root->ToMono(), cls.ToMono());

    ASSERT(managedInstance);

    // Execute default ctor
    mono_runtime_object_init(managedInstance);

    InitializeInstance(object, managedInstance, cls.ToMono());
}

void ObjectManager::InitializeInstance(Object* object, MonoObject* managedObject, MonoClass* managedObjectClass)
{
    ASSERT(object);
    ASSERT(managedObject);

    const auto gcHandle = mono_gchandle_new(managedObject, true);

    object->Initialize(gcHandle, managedObject, Class(managedObjectClass));
    object->SetNativePointer(object);

    // Register object
    RegisterObject(object);
}

void ObjectManager::Destroy(Object* objectInstance)
{
    if (!objectInstance->RemoveRef())
        return;

    ASSERT(objectInstance);
    objectInstance->SetNativePointer(nullptr);

    // Queue the object to destroy at the end of the current update frame
    GetInstance()->m_destroyQueue.enqueue(objectInstance);
}

void ObjectManager::DestroyNow(Object* objectInstance)
{
    if (!objectInstance->RemoveRef())
        return;

    ASSERT(objectInstance);
    objectInstance->SetNativePointer(nullptr);

    // Fully destroy the object now
    GetInstance()->DestroyObject(objectInstance);
}

MonoObject* ObjectManager::New(MonoType* type)
{
    // Get object creator
    const auto objectCreator = GetInstance()->GetObjectCreator(type);
    _ASSERT_(objectCreator, "Object is abstract and cannot be created using Object::New<>.");

    // TODO: Throw mono exception

    // Create object
    const auto nativeObject = objectCreator->Invoke(true);

    // Return managed pointer
    return nativeObject->ToManaged();
}

MonoObject* ObjectManager::NewGeneric(MonoType* baseType, MonoType* type, MonoObject* obj)
{
    // Get object creator
    const auto objectCreator = GetInstance()->GetObjectCreator(baseType);
    _ASSERT_(objectCreator, "Object is abstract and cannot be created using Object::NewGeneric<>.");

    // TODO: Throw mono exception

    // Create object
    const auto nativeObject = objectCreator->Invoke(false);

    const auto objectClass = mono_object_get_class(obj);

    // Setup generic object
    GetInstance()->InitializeInstance(nativeObject, obj, objectClass);

    // Return managed pointer
    return nativeObject->ToManaged();
}
