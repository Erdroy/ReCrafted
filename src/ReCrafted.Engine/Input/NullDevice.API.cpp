// ReCrafted (c) 2016-2018 Always Too Late

#include "NullDevice.h"

namespace Internal
{
    
}

void NullDevice::InitRuntime()
{
    API_FILE("Input/NullDevice.Gen.cs");
    {
        API_USING("ReCrafted.API.Core");
        API_USING("ReCrafted.API.Mathematics");

        API_COMMENT("NullDevice input device");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Input", "NullDevice", "InputDevice", PARTIAL, NOCONSTRUCTOR);
        {
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
