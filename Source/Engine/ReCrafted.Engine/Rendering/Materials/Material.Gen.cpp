// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Material.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void Material_Get_MaterialName1(Material* instance, MonoString** p_data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Material");
        const auto _returnValue = instance->MaterialName();
        *p_data = MONO_STRING_FROM_CSTR(_returnValue);
    }

    static void Material_Set_MaterialName1(Material* instance, MonoString** p_data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_data);
        const auto data = MONO_STRING_TO_CSTR(*p_data);
        MONO_CHECK_OBJECT(instance, "Material");
        instance->MaterialName(data);
        MONO_FREE(data);
    }
    
    static void Material_Get_Shader2(Material* instance, MonoObject** data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "Material");
        const auto _fRetValue = instance->GetShader();
        const auto _returnValue = _fRetValue != nullptr ? _fRetValue ->ToManaged() : nullptr;
        *data = _returnValue;
    }
    
    static Object* Material_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'Material' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<Material>();
    }
};

void Material::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::Material_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(Material);
    API_BIND("ReCrafted.API.Rendering.Material::Get_InternalMaterialName", &APIProxy::Material_Get_MaterialName1);
    API_BIND("ReCrafted.API.Rendering.Material::Set_InternalMaterialName", &APIProxy::Material_Set_MaterialName1);
    API_BIND("ReCrafted.API.Rendering.Material::Get_InternalShader", &APIProxy::Material_Get_Shader2);
}

const char* Material::Fullname() 
{
    return "ReCrafted.API.Rendering.Material";
}

const char* Material::Name() 
{
    return "Material";
}

const char* Material::Namespace() 
{
    return "ReCrafted.API.Rendering";
}