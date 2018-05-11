// ReCrafted (c) 2016-2018 Always Too Late

#include "Logger.h"
#include "Scripting/Method.h"
#include "Scripting/Object.h"

SINGLETON_IMPL(Logger)

void Logger::onInit()
{
    m_api_log_callback = Object::findStaticMethod("ReCrafted.API.Core.Logger::OnEngineLog");
    m_api_log_shutdown = Object::findStaticMethod("ReCrafted.API.Core.Logger::Shutdown");
}

void Logger::onDispose()
{
    m_api_log_shutdown->invokeStatic();
}

void Logger::invokeCallback(const char* message, LogLevel::Enum logLevel)
{
    if (m_api_log_callback)
    {
        cvar string = mono_string_new(mono_domain_get(), message);
        void* params[] = {string, &logLevel};
        m_api_log_callback->invokeStatic(params);
    }
}
