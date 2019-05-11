// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Logger.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void Logger_WriteLog1(LogLevel level, MonoString* p_str) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_str);
        const auto str = MONO_STRING_TO_CSTR(p_str);
        Logger::WriteLog(level, str);
        MONO_FREE(str);
    }
};

void Logger::InitRuntime() 
{
    API_BIND("ReCrafted.API.Common.Logger::InternalWriteLog", &APIProxy::Logger_WriteLog1);
}

const char* Logger::Fullname() 
{
    return "ReCrafted.API.Common.Logger";
}

const char* Logger::Name() 
{
    return "Logger";
}

const char* Logger::Namespace() 
{
    return "ReCrafted.API.Common";
}