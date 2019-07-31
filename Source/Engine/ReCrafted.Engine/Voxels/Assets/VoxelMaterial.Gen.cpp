// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "VoxelMaterial.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static MonoObject* VoxelMaterial_GetTexture1(VoxelMaterial* instance, const VoxelMaterialType type) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelMaterial");
        const auto _returnValue = instance->GetTexture(type);
        return _returnValue != nullptr ? _returnValue->ToManaged() : nullptr;
    }
    
    static void VoxelMaterial_Get_Name1(VoxelMaterial* instance, MonoString** p_data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelMaterial");
        const auto _returnValue = instance->GetName();
        *p_data = MONO_STRING_FROM_CSTR(_returnValue);
    }
    
    static void VoxelMaterial_Get_Material2(VoxelMaterial* instance, VoxelMaterial_t* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelMaterial");
        const auto _returnValue = instance->GetMaterial();
        *data = _returnValue;
    }
    
    static void VoxelMaterial_Get_Hardness3(VoxelMaterial* instance, VoxelHardness_t* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelMaterial");
        const auto _returnValue = instance->GetHardness();
        *data = _returnValue;
    }
    
    static void VoxelMaterial_Get_HasNormalTextures4(VoxelMaterial* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelMaterial");
        const auto _returnValue = instance->HasNormalTextures();
        *data = _returnValue;
    }
    
    static void VoxelMaterial_Get_HasFarTextures5(VoxelMaterial* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelMaterial");
        const auto _returnValue = instance->HasFarTextures();
        *data = _returnValue;
    }
    
    static Object* VoxelMaterial_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'VoxelMaterial' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<VoxelMaterial>();
    }
};

void VoxelMaterial::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::VoxelMaterial_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(VoxelMaterial);
    API_BIND("ReCrafted.API.Voxels.VoxelMaterial::InternalGetTexture", &APIProxy::VoxelMaterial_GetTexture1);
    API_BIND("ReCrafted.API.Voxels.VoxelMaterial::Get_InternalName", &APIProxy::VoxelMaterial_Get_Name1);
    API_BIND("ReCrafted.API.Voxels.VoxelMaterial::Get_InternalMaterial", &APIProxy::VoxelMaterial_Get_Material2);
    API_BIND("ReCrafted.API.Voxels.VoxelMaterial::Get_InternalHardness", &APIProxy::VoxelMaterial_Get_Hardness3);
    API_BIND("ReCrafted.API.Voxels.VoxelMaterial::Get_InternalHasNormalTextures", &APIProxy::VoxelMaterial_Get_HasNormalTextures4);
    API_BIND("ReCrafted.API.Voxels.VoxelMaterial::Get_InternalHasFarTextures", &APIProxy::VoxelMaterial_Get_HasFarTextures5);
}

const char* VoxelMaterial::Fullname() 
{
    return "ReCrafted.API.Voxels.VoxelMaterial";
}

const char* VoxelMaterial::Name() 
{
    return "VoxelMaterial";
}

const char* VoxelMaterial::Namespace() 
{
    return "ReCrafted.API.Voxels";
}