// ReCrafted (c) 2016-2018 Always Too Late

#include "VoxelMaterial.h"
#include "Scripting/ScriptingAPI.h"

namespace Internal
{
    API_DEFINE_OBJECT_CREATOR("ReCrafted.API.Voxels", "VoxelMaterial", VoxelMaterial)
}

void VoxelMaterial::InitRuntime()
{
    API_REGISTER_OBJECT("ReCrafted.API.Voxels.VoxelMaterial", &Internal::CreateVoxelMaterial);

    API_FILE("Voxels/VoxelMaterial.Gen.cs");
    {
        API_COMMENT("VoxelMaterial class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Voxels", "VoxelMaterial", "Object", PARTIAL, NOCONSTRUCTOR);
        {
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
