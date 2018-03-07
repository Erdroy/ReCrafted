// ReCrafted (c) 2016-2018 Always Too Late

#include "Renderer.h"

extern Renderer* g_rendererInstance;

void Renderer::initRuntime()
{
    API_FILE("Graphics/Renderer.Gen.cs");
    {
        API_COMMENT("Renderer class.");
        API_CLASS(PUBLIC, STATIC, "ReCrafted.API.Graphics", "Renderer");
        {
        }
        API_CLASS_END()
    }
    API_FILE_END()
}