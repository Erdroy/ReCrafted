// ReCrafted (c) 2016-2019 Always Too Late

#include "Asset.h"
#include "Scripting/ScriptingAPI.h"

void Asset::InitRuntime()
{
    API_FILE("Content/Asset.Gen.cs");
    {
        API_COMMENT("Asset class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Content", "Asset", "Object", PARTIAL, NOCONSTRUCTOR);
        {

        }
        API_CLASS_END();
    }
    API_FILE_END();
}
