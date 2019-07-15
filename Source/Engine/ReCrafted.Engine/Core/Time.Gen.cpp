// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Time.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static float Time_CurrentTime1() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = Time::CurrentTime();
        return _returnValue;
    }
    
    static float Time_CurrentFixedTime2() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = Time::CurrentFixedTime();
        return _returnValue;
    }
    
    static float Time_DeltaTime3() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = Time::DeltaTime();
        return _returnValue;
    }
    
    static float Time_FixedDeltaTime4() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = Time::FixedDeltaTime();
        return _returnValue;
    }
    
    static float Time_FrameAlpha5() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = Time::FrameAlpha();
        return _returnValue;
    }
    
    static uint32_t Time_CurrentFrame6() 
    {
        MAIN_THREAD_ONLY();
        const auto _returnValue = Time::CurrentFrame();
        return _returnValue;
    }
};

void Time::InitRuntime() 
{
    MONO_REGISTER_OBJECT_TYPE(Time);
    API_BIND("ReCrafted.API.Core.Time::InternalCurrentTime", &APIProxy::Time_CurrentTime1);
    API_BIND("ReCrafted.API.Core.Time::InternalCurrentFixedTime", &APIProxy::Time_CurrentFixedTime2);
    API_BIND("ReCrafted.API.Core.Time::InternalDeltaTime", &APIProxy::Time_DeltaTime3);
    API_BIND("ReCrafted.API.Core.Time::InternalFixedDeltaTime", &APIProxy::Time_FixedDeltaTime4);
    API_BIND("ReCrafted.API.Core.Time::InternalFrameAlpha", &APIProxy::Time_FrameAlpha5);
    API_BIND("ReCrafted.API.Core.Time::InternalCurrentFrame", &APIProxy::Time_CurrentFrame6);
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