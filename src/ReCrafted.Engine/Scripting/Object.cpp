// ReCrafted (c) 2016-2018 Always Too Late

#include "Object.h"

#include "Core/Logger.h"
#include "Graphics/Camera.h"
#include "Method.h"
#include "Field.h"

std::atomic<ObjectId_t> Object::m_lastObjectId;
std::atomic<ObjectId_t> Object::m_objectCount;
spp::sparse_hash_map<ObjectId_t, Object*> Object::m_objectMap;
Lock Object::m_objectMapLock;

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

    // Initialize instance
    InitializeInstance(object, objectInstance, isObject);

    return objectInstance;
}

void Object::InitializeInstance(Object* object, MonoObject* instance, bool isObject)
{
    mono_runtime_object_init(instance);

    // Setup object instance
    object->m_gchandle = mono_gchandle_new(instance, true); // get garbage collector handle, and mark it pinned
    object->m_object = instance;
    object->m_class = mono_object_get_class(instance);

    // Set NativePtr field in managed object if this object inherits Object object.
    if(isObject)
    {
        // Set native pointer
        const auto testField = object->FindField("NativePtr");
        testField->SetValue(&object);
    }

    // Register object
    RegisterObject(object);
}

void Object::RegisterObject(Object* object)
{
    ScopeLock(m_objectMapLock);

    // Setup object id
    cvar objectId = m_lastObjectId++;
    object->m_id = objectId;

    // Add object to object map
    m_objectMap.insert(std::make_pair(objectId, object));

    // Increment object count
    ++m_objectCount;
}

void Object::Destroy(Object* object)
{
    // Call on destroy event
    object->OnDestroy();

    // Free garbage collector handle
    mono_gchandle_free(object->m_gchandle);
    object->m_gchandle = 0u;

    m_objectMapLock.LockNow();

    // Unregister
    cvar objectIterator = m_objectMap.find(object->m_id);
    
    if (objectIterator == m_objectMap.end())
    {
        Logger::LogWarning("Destroying object which is already destroyed (or invalid)! Pointer: {0}", reinterpret_cast<uint64_t>(object));
    }
    else
    {
        m_objectMap.erase(objectIterator);

        // Decrement object count
        --m_objectCount;
    }

    // Reset object's managed data
    object->m_gchandle = 0u;
    object->m_object = nullptr;
    object->m_class = nullptr;

    m_objectMapLock.UnlockNow();
}

void Object::Release(Object* object)
{
    ASSERT(object);
    ASSERT(IsObjectInitialized(object));

    // free garbage collector handle
    mono_gchandle_free(object->m_gchandle);
    object->m_gchandle = 0u;
}

void Object::UnbindManaged(Object* object)
{
    // set native pointer
    cvar nativePtr = object->FindField("NativePtr");
    nativePtr->SetValue(nullptr);
}

void Object::DestroyAll()
{
    ScopeLock(m_objectMapLock);

    for(rvar objectPair : m_objectMap)
    {
        cvar object = objectPair.second;

        // Call OnDestroy manually
        object->OnDestroy();

        // Free garbage collector handle
        mono_gchandle_free(object->m_gchandle);
    }

    // Clear object map
    m_objectMap.clear();

    // Reset object count
    m_objectCount = 0u;
}

void Object::Finalize(Object* object)
{
    ScopeLock(m_objectMapLock);

    cvar objectIterator = m_objectMap.find(object->m_id);

    if (objectIterator != m_objectMap.end())
    {
        // Remove object if not finalized
        m_objectMap.erase(objectIterator);
        object->OnDestroy();
        Logger::LogWarning("Object got finalized, but not destroyed at first!");
    }

    // cleanup
    object->m_object = nullptr;

    // Delete unmanaged object
    delete object;
}
