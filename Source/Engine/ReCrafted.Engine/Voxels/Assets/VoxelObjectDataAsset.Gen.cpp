// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "VoxelObjectDataAsset.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static Object* VoxelObjectDataAsset_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'VoxelObjectDataAsset' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<VoxelObjectDataAsset>();
    }
};

void VoxelObjectDataAsset::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::VoxelObjectDataAsset_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(VoxelObjectDataAsset);
}

const char* VoxelObjectDataAsset::Fullname() 
{
    return "ReCrafted.API.Voxels.VoxelObjectDataAsset";
}

const char* VoxelObjectDataAsset::Name() 
{
    return "VoxelObjectDataAsset";
}

const char* VoxelObjectDataAsset::Namespace() 
{
    return "ReCrafted.API.Voxels";
}