// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Script.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static MonoObject* Script_Actor1(Script* instance) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Script");
        const auto _returnValue = instance->Actor();
        return _returnValue != nullptr ? _returnValue->ToManaged() : nullptr;
    }
    
    static void Script_Get_Enabled1(Script* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Script");
        const auto _returnValue = instance->Enabled();
        *data = _returnValue;
    }

    static void Script_Set_Enabled1(Script* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Script");
        instance->Enabled(*data);
    }
    
    static Object* Script_CreateObject(bool createManagedInstance)
    {
        return createManagedInstance ? Object::New<Script>() : new Script();
    }
};

void Script::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::Script_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(Script);
    API_BIND("ReCrafted.API.Script::InternalActor", &APIProxy::Script_Actor1);
    API_BIND("ReCrafted.API.Script::Get_InternalEnabled", &APIProxy::Script_Get_Enabled1);
    API_BIND("ReCrafted.API.Script::Set_InternalEnabled", &APIProxy::Script_Set_Enabled1);
}

const char* Script::Fullname() 
{
    return "ReCrafted.API.Script";
}

const char* Script::Name() 
{
    return "Script";
}

const char* Script::Namespace() 
{
    return "ReCrafted.API";
}