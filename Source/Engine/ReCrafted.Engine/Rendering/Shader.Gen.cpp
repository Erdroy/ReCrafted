// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Shader.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void Shader_SetTexture1(Shader* instance, const int slot, Texture* texture) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Shader");
        instance->SetTexture(slot, texture);
    }
    
    static Object* Shader_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'Shader' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<Shader>();
    }
};

void Shader::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::Shader_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(Shader);
    API_BIND("ReCrafted.API.Rendering.Shader::InternalSetTexture", &APIProxy::Shader_SetTexture1);
}

const char* Shader::Fullname() 
{
    return "ReCrafted.API.Rendering.Shader";
}

const char* Shader::Name() 
{
    return "Shader";
}

const char* Shader::Namespace() 
{
    return "ReCrafted.API.Rendering";
}