// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Display.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void Display_Get_AspectRatio1(float* data) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = Display::GetAspectRatio();
        *data = _returnValue;
    }
    
    static void Display_Get_Width2(uint* data) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = Display::GetWidth();
        *data = _returnValue;
    }
    
    static void Display_Get_Height3(uint* data) 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = Display::GetHeight();
        *data = _returnValue;
    }
};

void Display::InitRuntime() 
{
    MONO_REGISTER_OBJECT_TYPE(Display);
    API_BIND("ReCrafted.API.Core.Display::Get_InternalAspectRatio", &APIProxy::Display_Get_AspectRatio1);
    API_BIND("ReCrafted.API.Core.Display::Get_InternalWidth", &APIProxy::Display_Get_Width2);
    API_BIND("ReCrafted.API.Core.Display::Get_InternalHeight", &APIProxy::Display_Get_Height3);
}

const char* Display::Fullname() 
{
    return "ReCrafted.API.Core.Display";
}

const char* Display::Name() 
{
    return "Display";
}

const char* Display::Namespace() 
{
    return "ReCrafted.API.Core";
}