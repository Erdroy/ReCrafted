// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Object.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void Object_Destroy(Object* objectInstance) 
    {
        MAIN_THREAD_ONLY();
        Object::Destroy(objectInstance);
    }
    
    static void Object_DestroyNow(Object* objectInstance) 
    {
        MAIN_THREAD_ONLY();
        Object::DestroyNow(objectInstance);
    }
    
    static MonoObject* Object_New(MonoType* type) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = Object::New(type);
        return _returnValue;
    }
    
    static MonoObject* Object_NewGeneric(MonoType* baseType, MonoObject* obj) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = Object::NewGeneric(baseType, obj);
        return _returnValue;
    }
};

void Object::InitRuntime() 
{
    API_BIND("ReCrafted.API.Object::InternalDestroy", &APIProxy::Object_Destroy);
    API_BIND("ReCrafted.API.Object::InternalDestroyNow", &APIProxy::Object_DestroyNow);
    API_BIND("ReCrafted.API.Object::InternalNew", &APIProxy::Object_New);
    API_BIND("ReCrafted.API.Object::InternalNewGeneric", &APIProxy::Object_NewGeneric);
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