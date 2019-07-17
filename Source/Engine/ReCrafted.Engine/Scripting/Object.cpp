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
    if (!m_class.IsValid())
        return;

    // Set NativePtr field in managed object if the object does have it.
    const auto testField = m_class.GetField("NativePtr");
    auto pointerValue = reinterpret_cast<uintptr_t>(pointer);

    if (testField.IsValid())
        testField.SetValue(this, &pointerValue);
}

void Object::AddRef()
{
    m_refCount++;
}

bool Object::RemoveRef()
{
    if (m_refCount == 0u)
        return true;

    m_refCount--;
    return m_refCount == 0u;
}

void Object::ResetRefCount()
{
    m_refCount = 1u;
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

MonoObject* Object::NewGeneric(MonoType* baseType, MonoType* type, MonoObject* obj)
{
    return ObjectManager::NewGeneric(baseType, type, obj);
}
