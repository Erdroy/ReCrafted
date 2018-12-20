// ReCrafted (c) 2016-2018 Always Too Late

#include "Mouse.h"

namespace Internal
{

}

void Mouse::InitRuntime()
{
    API_FILE("Input/Mouse.Gen.cs");
    {
        API_USING("ReCrafted.API.Core");
        API_USING("ReCrafted.API.Mathematics");

        API_COMMENT("Mouse input device");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Input", "Mouse", "InputDevice", PARTIAL, NOCONSTRUCTOR);
        {
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
