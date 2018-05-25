// ReCrafted (c) 2016-2018 Always Too Late

#include "Application.h"
#include "Scripting/Mono.h"
#include "EngineMain.h"
#include "UpdateLoop.h"
#include "Platform/Platform.h"

namespace Internal
{
    int getTargetFps()
    {
        return EngineMain::GetInstance()->GetUpdateLoop()->GetTargetFps();
    }

    void setTargetFps(int targetFps)
    {
        EngineMain::GetInstance()->GetUpdateLoop()->SetTargetFps(targetFps);
    }

    int getCursorIcon()
    {
        return Platform::GetCursorIcon();
    }

    void setCursorIcon(int iconId)
    {
        Platform::SetCursorIcon(iconId);
    }
}

void Application::InitRuntime()
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

            API_COMMENT("The target amount of frames per second (FPS).");
            API_PROPERTY(PUBLIC, STATIC, "int", "TargetFps", GETSET);
            {
                API_BIND("ReCrafted.API.Core.Application::Internal_TargetFps_Get", &Internal::getTargetFps);
                API_BIND("ReCrafted.API.Core.Application::Internal_TargetFps_Set", &Internal::setTargetFps);
            }
            API_PROPERTY_END();

            API_COMMENT("Enables or disables vertical synchronization (vsync).");
            API_PROPERTY(PUBLIC, STATIC, "bool", "VSync", GETSET);
            {
                // TODO: implement VSync enable/disable
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets current cursor icon.");
            API_PROPERTY(PUBLIC, STATIC, "int", "CursorIcon", GETSET);
            {
                API_BIND("ReCrafted.API.Core.Application::Internal_CursorIcon_Get", &Internal::getCursorIcon);
                API_BIND("ReCrafted.API.Core.Application::Internal_CursorIcon_Set", &Internal::setCursorIcon);
            }
            API_PROPERTY_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
