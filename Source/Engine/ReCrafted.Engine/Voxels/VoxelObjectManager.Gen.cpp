// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "VoxelObjectManager.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
};

void VoxelObjectManager::InitRuntime() 
{
    MONO_REGISTER_OBJECT_TYPE(VoxelObjectManager);
}

const char* VoxelObjectManager::Fullname() 
{
    return "ReCrafted.API.Voxels.VoxelObjectManager";
}

const char* VoxelObjectManager::Name() 
{
    return "VoxelObjectManager";
}

const char* VoxelObjectManager::Namespace() 
{
    return "ReCrafted.API.Voxels";
}