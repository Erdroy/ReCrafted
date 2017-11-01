// ReCrafted © 2016-2017 Always Too Late

#include "Script.h"

void Script::initRuntime()
{
    API_FILE("Common/Script.Gen.cs")
    {
        API_COMMENT("Script base class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Common", "Script", "Object", PARTIAL);
        {

        }
        API_CLASS_END();
    }
    API_FILE_END();
}
