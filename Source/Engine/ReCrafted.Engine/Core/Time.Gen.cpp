// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Time.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:

    static float Time_CurrentTime() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = Time::CurrentTime();
        return _returnValue;
    }

    static float Time_CurrentFixedTime() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = Time::CurrentFixedTime();
        return _returnValue;
    }

    static float Time_DeltaTime() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = Time::DeltaTime();
        return _returnValue;
    }

    static float Time_FixedDeltaTime() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = Time::FixedDeltaTime();
        return _returnValue;
    }

    static uint32_t Time_CurrentFrame() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = Time::CurrentFrame();
        return _returnValue;
    }
};

void Time::InitRuntime() 
{
    API_BIND("ReCrafted.API.Core.Time::InternalCurrentTime", &APIProxy::Time_CurrentTime);
    API_BIND("ReCrafted.API.Core.Time::InternalCurrentFixedTime", &APIProxy::Time_CurrentFixedTime);
    API_BIND("ReCrafted.API.Core.Time::InternalDeltaTime", &APIProxy::Time_DeltaTime);
    API_BIND("ReCrafted.API.Core.Time::InternalFixedDeltaTime", &APIProxy::Time_FixedDeltaTime);
    API_BIND("ReCrafted.API.Core.Time::InternalCurrentFrame", &APIProxy::Time_CurrentFrame);
}

const char* Time::Fullname() 
{
    return "ReCrafted.API.Core.Time";
}

const char* Time::Name() 
{
    return "Time";
}

const char* Time::Namespace() 
{
    return "ReCrafted.API.Core";
}