// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "VoxelObjectBase.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void VoxelObjectBase_Initialize1(VoxelObjectBase* instance) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "VoxelObjectBase");
        instance->Initialize();
    }
};

void VoxelObjectBase::InitRuntime() 
{
    MONO_REGISTER_OBJECT_TYPE(VoxelObjectBase);
    API_BIND("ReCrafted.API.Voxels.VoxelObjectBase::InternalInitialize", &APIProxy::VoxelObjectBase_Initialize1);
}

const char* VoxelObjectBase::Fullname() 
{
    return "ReCrafted.API.Voxels.VoxelObjectBase";
}

const char* VoxelObjectBase::Name() 
{
    return "VoxelObjectBase";
}

const char* VoxelObjectBase::Namespace() 
{
    return "ReCrafted.API.Voxels";
}