// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "VoxelMaterialManager.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void VoxelMaterialManager_AddMaterial1(VoxelMaterial* material) 
    {
        MAIN_THREAD_ONLY();
        VoxelMaterialManager::AddMaterial(material);
    }
    
    static void VoxelMaterialManager_SetDefaultMaterial2(VoxelMaterial* material) 
    {
        MAIN_THREAD_ONLY();
        VoxelMaterialManager::SetDefaultMaterial(material);
    }
    
    static MonoObject* VoxelMaterialManager_GetMaterial3(VoxelMaterial_t id) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = VoxelMaterialManager::GetMaterial(id);
        return _returnValue != nullptr ? _returnValue->ToManaged() : nullptr;
    }
};

void VoxelMaterialManager::InitRuntime() 
{
    MONO_REGISTER_OBJECT_TYPE(VoxelMaterialManager);
    API_BIND("ReCrafted.API.Voxels.VoxelMaterialManager::InternalAddMaterial", &APIProxy::VoxelMaterialManager_AddMaterial1);
    API_BIND("ReCrafted.API.Voxels.VoxelMaterialManager::InternalSetDefaultMaterial", &APIProxy::VoxelMaterialManager_SetDefaultMaterial2);
    API_BIND("ReCrafted.API.Voxels.VoxelMaterialManager::InternalGetMaterial", &APIProxy::VoxelMaterialManager_GetMaterial3);
}

const char* VoxelMaterialManager::Fullname() 
{
    return "ReCrafted.API.Voxels.VoxelMaterialManager";
}

const char* VoxelMaterialManager::Name() 
{
    return "VoxelMaterialManager";
}

const char* VoxelMaterialManager::Namespace() 
{
    return "ReCrafted.API.Voxels";
}