// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Shader.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void Shader_SetTextureArray1(Shader* instance, const int slot, MonoArray* p_textureArray) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_textureArray);
        const auto textureArray = MONO_ARRAY_TO_OBJECT_ARRAY(p_textureArray, Texture, Texture*, MonoObject*, _t0->ToManaged());
        MONO_CHECK_OBJECT(instance, "Shader");
        instance->SetTextureArray(slot, textureArray);
        MONO_FREE_ARRAY(textureArray);
    }
    
    static void Shader_SetTexture2(Shader* instance, const int slot, Texture* texture) 
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
    API_BIND("ReCrafted.API.Rendering.Shader::InternalSetTextureArray", &APIProxy::Shader_SetTextureArray1);
    API_BIND("ReCrafted.API.Rendering.Shader::InternalSetTexture", &APIProxy::Shader_SetTexture2);
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