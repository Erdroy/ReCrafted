// ReCrafted (c) 2016-2018 Always Too Late

#include "Logger.h"
#include "Scripting/Method.h"
#include "Scripting/Object.h"

SINGLETON_IMPL(Logger)

void Logger::OnInit()
{
    m_api_log_callback = Object::FindStaticMethod("ReCrafted.API.Core.Logger::OnEngineLog");
    m_api_log_shutdown = Object::FindStaticMethod("ReCrafted.API.Core.Logger::Shutdown");
}

void Logger::OnDispose()
{
    m_api_log_shutdown->InvokeStatic();
}

void Logger::InvokeCallback(const char* message, LogLevel::Enum logLevel)
{
    Platform::Log(message);

    if (m_api_log_callback)
    {
        cvar string = mono_string_new(mono_domain_get(), message);
        void* params[] = {string, &logLevel};
        m_api_log_callback->InvokeStatic(params);
    }
}
