// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "EmptyActor.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static MonoObject* EmptyActor_Create() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = EmptyActor::Create();
        return _returnValue != nullptr ? _returnValue->ToManaged() : nullptr;
    }
    
    static Object* EmptyActor_CreateObject(bool createManagedInstance)
    {
        return createManagedInstance ? Object::New<EmptyActor>() : new EmptyActor();
    }
};

void EmptyActor::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::EmptyActor_CreateObject);
    
    API_BIND("ReCrafted.API.Core.Actors.EmptyActor::InternalCreate", &APIProxy::EmptyActor_Create);
}

const char* EmptyActor::Fullname() 
{
    return "ReCrafted.API.Core.Actors.EmptyActor";
}

const char* EmptyActor::Name() 
{
    return "EmptyActor";
}

const char* EmptyActor::Namespace() 
{
    return "ReCrafted.API.Core.Actors";
}