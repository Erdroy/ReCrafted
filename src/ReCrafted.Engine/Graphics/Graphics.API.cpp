// ReCrafted (c) 2016-2018 Always Too Late

#include "Graphics.h"

extern Graphics* g_rendererInstance;

void Graphics::InitRuntime()
{
    API_FILE("Graphics/Graphics.Gen.cs");
    {
        API_COMMENT("Graphics class.");
        API_CLASS(PUBLIC, STATIC, "ReCrafted.API.Graphics", "Graphics");
        {
        }
        API_CLASS_END()
    }
    API_FILE_END()
}
