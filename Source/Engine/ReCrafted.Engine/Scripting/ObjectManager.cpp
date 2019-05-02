// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "ObjectManager.h"
#include "Mono.h"
#include "Assembly.h"
#include "Class.h"
#include "Domain.h"
#include "Object.h"
#include "Field.h"

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

void ObjectManager::Initialize()
{
}

void ObjectManager::Shutdown()
{
}

void ObjectManager::InitializeInstance(Object* object, const char* nameSpace, const char* name, const RefPtr<Assembly>& assembly, const RefPtr<Domain>& domain)
{
    MAIN_THREAD_ONLY();

    const auto cls = assembly->GetClass(nameSpace, name);

    _ASSERT_(cls.IsValid(), "Could not find class type. Make sure that the namespace and class name does exist in the given assembly.");

    const auto managedInstance = mono_object_new(domain ? domain->ToMono() : Domain::Root->ToMono(), cls.ToMono());
    ASSERT(managedInstance);

    const auto gcHandle = mono_gchandle_new(managedInstance, true);

    object->Initialize(gcHandle, managedInstance, cls);
    object->SetNativePointer(object);

    // Register object
    RegisterObject(object);
}

void ObjectManager::Destroy(Object* objectInstance)
{
    // TODO: Destroy object
}

void ObjectManager::DestroyNow(Object* objectInstance)
{
    // TODO: Destroy object now

    // Delete object
    delete objectInstance;
}
