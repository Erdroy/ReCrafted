// ReCrafted (c) 2016-2019 Always Too Late

#include "Display.h"
#include "Scripting/Mono.h"
#include "Common/ReCraftedAPI.h"

namespace Internal
{
    int Width()
    {
        return static_cast<int>(Display::GetWidth());
    }

    int Height()
    {
        return static_cast<int>(Display::GetHeight());
    }
}

void Display::InitRuntime()
{
    API_FILE("Common/Display.Gen.cs");
    {
        API_COMMENT("Display class.");
        API_CLASS(PUBLIC, STATIC, "ReCrafted.API.Common", "Display");
        {
            API_COMMENT("Returns display's width");
            API_PROPERTY(PUBLIC, STATIC, "int", "Width", GET);
            {
                API_BIND("ReCrafted.API.Common.Display::Internal_Width_Get", &Internal::Width);
            }
            API_PROPERTY_END();

            API_COMMENT("Returns display's height");
            API_PROPERTY(PUBLIC, STATIC, "int", "Height", GET);
            {
                API_BIND("ReCrafted.API.Common.Display::Internal_Height_Get", &Internal::Height);
            }
            API_PROPERTY_END();

            API_COMMENT("Returns display's aspect ratio");
            API_PROPERTY(PUBLIC, STATIC, "float", "AspectRatio", GET);
            {
                API_BIND("ReCrafted.API.Common.Display::Internal_AspectRatio_Get", &Display::GetAspectRatio);
            }
            API_PROPERTY_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
