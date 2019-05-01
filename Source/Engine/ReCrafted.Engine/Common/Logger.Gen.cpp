// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "Logger.h"
#include "Scripting/Mono.h"

class APIProxy
{
public:

    static void Logger_WriteLog(LogLevel level, MonoString* p_str) 
    {
        MAIN_THREAD_ONLY();
        auto str = MONO_STRING_TO_CSTR(p_str);
        Logger::WriteLog(level, str);
    }
};

void Logger::InitRuntime() 
{
    API_BIND("ReCrafted.API.Common.Logger::InternalWriteLog", &APIProxy::Logger_WriteLog);
}