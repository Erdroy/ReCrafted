// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Profiler.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static uint32_t Profiler_BeginCPUProfile1(MonoString* p_name) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_name);
        const auto name = MONO_STRING_TO_CSTR(p_name);
        const auto _returnValue = Profiler::BeginCPUProfile(name);
        MONO_FREE(name);
        return _returnValue;
    }
    
    static void Profiler_EndCPUProfile2(const uint32_t profileId) 
    {
        MAIN_THREAD_ONLY();
        Profiler::EndCPUProfile(profileId);
    }
    
    static void Profiler_BeginProfile3(MonoString* p_name) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_name);
        const auto name = MONO_STRING_TO_CSTR(p_name);
        Profiler::BeginProfile(name);
        MONO_FREE(name);
    }
    
    static void Profiler_EndProfile4() 
    {
        MAIN_THREAD_ONLY();
        Profiler::EndProfile();
    }
};

void Profiler::InitRuntime() 
{
    MONO_REGISTER_OBJECT_TYPE(Profiler);
    API_BIND("ReCrafted.API.Core.Profiler::InternalBeginCPUProfile", &APIProxy::Profiler_BeginCPUProfile1);
    API_BIND("ReCrafted.API.Core.Profiler::InternalEndCPUProfile", &APIProxy::Profiler_EndCPUProfile2);
    API_BIND("ReCrafted.API.Core.Profiler::InternalBeginProfile", &APIProxy::Profiler_BeginProfile3);
    API_BIND("ReCrafted.API.Core.Profiler::InternalEndProfile", &APIProxy::Profiler_EndProfile4);
}

const char* Profiler::Fullname() 
{
    return "ReCrafted.API.Core.Profiler";
}

const char* Profiler::Name() 
{
    return "Profiler";
}

const char* Profiler::Namespace() 
{
    return "ReCrafted.API.Core";
}