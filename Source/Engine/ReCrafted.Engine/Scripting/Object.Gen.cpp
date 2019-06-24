// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Object.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void Object_AddRef1(Object* instance) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Object");
        instance->AddRef();
    }
    
    static bool Object_RemoveRef2(Object* instance) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Object");
        const auto _returnValue = instance->RemoveRef();
        return _returnValue;
    }
    
    static uint32_t Object_GetRefCount3(Object* instance) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Object");
        const auto _returnValue = instance->GetRefCount();
        return _returnValue;
    }
    
    static void Object_Destroy4(Object* objectInstance) 
    {
        Object::Destroy(objectInstance);
    }
    
    static void Object_DestroyNow5(Object* objectInstance) 
    {
        Object::DestroyNow(objectInstance);
    }
    
    static MonoObject* Object_New6(MonoType* type) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = Object::New(type);
        return _returnValue;
    }
    
    static MonoObject* Object_NewGeneric7(MonoType* baseType, MonoType* type, MonoObject* obj) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = Object::NewGeneric(baseType, type, obj);
        return _returnValue;
    }
};

void Object::InitRuntime() 
{
    MONO_REGISTER_OBJECT_TYPE(Object);
    API_BIND("ReCrafted.API.Object::InternalAddRef", &APIProxy::Object_AddRef1);
    API_BIND("ReCrafted.API.Object::InternalRemoveRef", &APIProxy::Object_RemoveRef2);
    API_BIND("ReCrafted.API.Object::InternalGetRefCount", &APIProxy::Object_GetRefCount3);
    API_BIND("ReCrafted.API.Object::InternalDestroy", &APIProxy::Object_Destroy4);
    API_BIND("ReCrafted.API.Object::InternalDestroyNow", &APIProxy::Object_DestroyNow5);
    API_BIND("ReCrafted.API.Object::InternalNew", &APIProxy::Object_New6);
    API_BIND("ReCrafted.API.Object::InternalNewGeneric", &APIProxy::Object_NewGeneric7);
}

const char* Object::Fullname() 
{
    return "ReCrafted.API.Object";
}

const char* Object::Name() 
{
    return "Object";
}

const char* Object::Namespace() 
{
    return "ReCrafted.API";
}