// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "ObjectManager.h"
#include "Mono.h"
#include "Assembly.h"
#include "Class.h"
#include "Domain.h"
#include "Object.h"
#include "Field.h"
#include "ScriptingManager.h"

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
    objectInstance->SetNativePointer(nullptr);

    // TODO: Destroy object
}

void ObjectManager::DestroyNow(Object* objectInstance)
{
    objectInstance->SetNativePointer(nullptr);

    // TODO: Destroy object now

    // Delete object
    delete objectInstance;
}

MonoObject* ObjectManager::New(MonoType* type)
{
    // Get object creator
    const auto objectCreator = GetInstance()->GetObjectCreator(type);
    _ASSERT_(objectCreator, "Object creator for this type is not implemented!");

    // Create object
    const auto nativeObject = objectCreator->Invoke(true);

    // Return managed pointer
    return nativeObject->ToManaged();
}

MonoObject* ObjectManager::NewGeneric(MonoType* baseType, MonoObject* obj)
{
    // Get object creator
    const auto objectCreator = GetInstance()->GetObjectCreator(baseType);
    _ASSERT_(objectCreator, "Object creator for this type is not implemented!");

    // Create object
    const auto nativeObject = objectCreator->Invoke(false);

    // Setup generic object
    GetInstance()->InitializeInstance(nativeObject, obj, nativeObject->GetClass().ToMono());

    // Return managed pointer
    return nativeObject->ToManaged();
}
