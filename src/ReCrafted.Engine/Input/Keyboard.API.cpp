// ReCrafted (c) 2016-2018 Always Too Late

#include "Keyboard.h"

namespace Internal
{

}

void Keyboard::InitRuntime()
{
    API_FILE("Input/Keyboard.Gen.cs");
    {
        API_USING("ReCrafted.API.Core");
        API_USING("ReCrafted.API.Mathematics");

        API_COMMENT("Keyboard input device");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Input", "Keyboard", "InputDevice", PARTIAL, NOCONSTRUCTOR);
        {
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
