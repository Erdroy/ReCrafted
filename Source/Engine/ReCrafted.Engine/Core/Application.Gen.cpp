// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Application.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void Application_Quit1() 
    {
        MAIN_THREAD_ONLY();
        Application::Quit();
    }
};

void Application::InitRuntime() 
{
    MONO_REGISTER_OBJECT_TYPE(Application);
    API_BIND("ReCrafted.API.Core.Application::InternalQuit", &APIProxy::Application_Quit1);
}

const char* Application::Fullname() 
{
    return "ReCrafted.API.Core.Application";
}

const char* Application::Name() 
{
    return "Application";
}

const char* Application::Namespace() 
{
    return "ReCrafted.API.Core";
}