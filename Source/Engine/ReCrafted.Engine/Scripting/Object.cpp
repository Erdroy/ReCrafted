// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Object.h"

void Object::Initialize(const GCHandle_t gcHandle, MonoObject* managedInstance, const Class& cls)
{
    m_gcHandle = gcHandle;
    m_managedInstance = managedInstance;
    m_class = cls;
}

bool Object::IsValid() const
{
    return m_class.IsValid();
}

Class Object::GetClass() const
{
    return m_class;
}

MonoObject* Object::ToManaged() const
{
    return m_managedInstance;
}

void Object::Destroy(Object* objectInstance)
{
    ObjectManager::Destroy(objectInstance);
}

void Object::DestroyNow(Object* objectInstance)
{
    ObjectManager::DestroyNow(objectInstance);
}
