// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "VoxelPlanet.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static Object* VoxelPlanet_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'VoxelPlanet' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<VoxelPlanet>();
    }
};

void VoxelPlanet::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::VoxelPlanet_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(VoxelPlanet);
}

const char* VoxelPlanet::Fullname() 
{
    return "ReCrafted.API.Voxels.VoxelPlanet";
}

const char* VoxelPlanet::Name() 
{
    return "VoxelPlanet";
}

const char* VoxelPlanet::Namespace() 
{
    return "ReCrafted.API.Voxels";
}