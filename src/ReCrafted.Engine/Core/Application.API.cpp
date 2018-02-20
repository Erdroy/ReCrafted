// ReCrafted (c) 2016-2018 Always Too Late

#include "Application.h"

Application* Application::m_instance;

void Application::initRuntime()
{
    API_FILE("Core/Application.Gen.cs");
    {
        API_COMMENT("Application class.");
        API_CLASS(PUBLIC, ABSTRACT, "ReCrafted.API.Core", "Application");
        {
            API_METHOD(PROTECTED, ABSTRACT, "Initialize");
            API_METHOD_END();

            API_METHOD(PROTECTED, ABSTRACT, "Shutdown");
            API_METHOD_END();

            API_METHOD(PROTECTED, ABSTRACT, "Update");
            API_METHOD_END();

            API_METHOD(PROTECTED, ABSTRACT, "Simulate");
            API_METHOD_END();

            API_METHOD(PROTECTED, ABSTRACT, "Render");
            API_METHOD_END();

            API_METHOD(PROTECTED, ABSTRACT, "RenderUI");
            API_METHOD_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
