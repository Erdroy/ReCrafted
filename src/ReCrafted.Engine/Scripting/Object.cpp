// ReCrafted (c) 2016-2018 Always Too Late

#include "Object.h"

#include "Core/Logger.h"
#include "Graphics/Camera.h"
#include "Method.h"
#include "Field.h"

Array<Object*> Object::m_objects;

RefPtr<Method> Object::FindMethod(const char* methodName) const
{
    auto methodDesc = mono_method_desc_new(methodName, true);

    if (!methodDesc)
        return nullptr;

    auto methodHandle = mono_method_desc_search_in_class(methodDesc, m_class);

    mono_method_desc_free(methodDesc);

    if (!methodHandle)
        return nullptr;

    RefPtr<Method> method(new Method);
    method->m_object = m_object;
    method->m_method = methodHandle;
    return method;
}

RefPtr<Field> Object::FindField(const char* fieldName) const
{
    auto fieldDesc = mono_class_get_field_from_name(m_class, fieldName);

    if (!fieldDesc)
        return nullptr;

    RefPtr<Field> field(new Field);

    field->m_object = m_object;
    field->m_field = fieldDesc;

    return field;
}

MonoObject* Object::GetManagedPtr() const
{
    return m_object;
}

RefPtr<Method> Object::FindStaticMethod(const char* methodName)
{
    auto methodDesc = mono_method_desc_new(methodName, true);
    if (!methodDesc)
        return nullptr;

    auto image = mono_assembly_get_image(Assembly::API.get()->m_assembly);
    auto methodHandle = mono_method_desc_search_in_image(methodDesc, image);

    mono_method_desc_free(methodDesc);

    if (!methodHandle)
        return nullptr;

    RefPtr<Method> method(new Method);
    method->m_object = nullptr;
    method->m_method = methodHandle;
    return method;
}

bool Object::IsObjectInitialized(Object* object)
{
    return object->m_object && object->m_class;
}

MonoObject* Object::Create(Object* object, MonoDomain* domain, MonoClass* monoClass, bool isObject)
{
    cvar objectInstance = mono_object_new(domain, monoClass);

    ASSERT(objectInstance);

    mono_runtime_object_init(objectInstance);

    // get garbage collector handle, and mark it pinned
    auto gch = mono_gchandle_new(objectInstance, true);

    object->m_gchandle = gch;
    object->m_object = objectInstance;
    object->m_class = monoClass;

    if (isObject)
    {
        // set native pointer
        auto testField = object->FindField("NativePtr");
        testField->SetValue((void*)&object);

        // register object
        RegisterObject(object);
    }

    return objectInstance;
}

void Object::InitializeInstance(Object* object, MonoObject* instance)
{
    mono_runtime_object_init(instance);

    // get garbage collector handle, and mark it pinned
    auto gch = mono_gchandle_new(instance, true);

    object->m_gchandle = gch;
    object->m_object = instance;
    object->m_class = mono_object_get_class(instance);

    // set native pointer
    auto testField = object->FindField("NativePtr");
    testField->SetValue(&object);

    // register object
    RegisterObject(object);
}

void Object::RegisterObject(Object* object)
{
    m_objects.Add(object);
}

void Object::Destroy(Object* object)
{
    // Call on destroy event
    object->OnDestroy();

    // free garbage collector handle
    mono_gchandle_free(object->m_gchandle);
    object->m_gchandle = 0u;

    // unregister
    m_objects.Remove(object);

    // Reset object's managed data
    object->m_gchandle = 0u;
    object->m_object = nullptr;
    object->m_class = nullptr;
}

void Object::DestroyAll()
{
    for (auto i = 0u; i < m_objects.Size(); i ++)
    {
        auto object = m_objects[i];

        object->OnDestroy();

        // free garbage collector handle
        mono_gchandle_free(object->m_gchandle);
    }

    m_objects.Clear();
}

void Object::Finalize(Object* object)
{
    if (m_objects.Count() > 0u && m_objects.Remove(object))
    {
        object->OnDestroy();
        Logger::LogWarning("Object was finalized, but not destroyed at first!");
    }

    // cleanup
    object->m_object = nullptr;
}
