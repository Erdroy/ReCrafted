// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Object.h"
#include "Class.h"
#include "Field.h"

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

void Object::SetNativePointer(void* pointer)
{
    // Set NativePtr field in managed object if the object does have it.
    const auto testField = m_class.GetField("NativePtr");
    
    if (testField.IsValid())
        testField.SetValue(this, pointer);
}

void Object::Destroy(Object* objectInstance)
{
    ObjectManager::Destroy(objectInstance);
}

void Object::DestroyNow(Object* objectInstance)
{
    ObjectManager::DestroyNow(objectInstance);
}

MonoObject* Object::New(MonoType* type)
{
    return ObjectManager::New(type);;
}

MonoObject* Object::NewGeneric(MonoType* baseType, MonoObject* obj)
{
    return ObjectManager::NewGeneric(baseType, obj);
}
