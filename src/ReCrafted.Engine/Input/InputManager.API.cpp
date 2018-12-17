// ReCrafted (c) 2016-2018 Always Too Late

#include "InputManager.h"

namespace Internal
{
    
}

void InputManager::InitRuntime()
{
    API_FILE("Input/InputManager.Gen.cs");
    {
        API_USING("ReCrafted.API.Core");
        API_USING("ReCrafted.API.Mathematics");

        API_COMMENT("InputManager class.");
        API_CLASS(PUBLIC, STATIC, "ReCrafted.API.Input", "InputManager");
        {
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
