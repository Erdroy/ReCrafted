// ReCrafted (c) 2016-2019 Always Too Late

#include "ActionMap.h"
#include "Scripting/ScriptingAPI.h"

namespace Internal
{
}

void ActionMap::InitRuntime()
{
    API_FILE("Input/ActionMap.Gen.cs");
    {
        API_USING("ReCrafted.API.Core");
        API_USING("ReCrafted.API.Mathematics");

        API_COMMENT("ActionMap class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Input", "ActionMap", "Object", PARTIAL, NOCONSTRUCTOR);
        {
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
