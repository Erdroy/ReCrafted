// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef LOGGER_H
#define LOGGER_H

// includes
#include "Common/Text.h"
#include "fmt/format.h"

#include "EngineComponent.h"

/**
 * \brief Log level enum, specifies the log level.
 */
struct LogLevel
{
    enum Enum
    {
        Message = 0,
        Warning,
        Error,
        Fatal,

        Count
    };
};

class Method;

/**
 * \brief Logger class - provides basic logging system.
 * Writes logs into log file or message event.
 */
class Logger : public EngineComponent<Logger>
{
private:
    friend class EngineMain;

private:
    RefPtr<Method> m_api_log_callback = nullptr;
    RefPtr<Method> m_api_log_shutdown = nullptr;

public:
    ~Logger() = default;

private:
    void OnInit() override;
    void OnDispose() override;

private:
    void InvokeCallback(const char* message, LogLevel::Enum logLevel);

public:
    /**
     * \brief Writes exception message into log file.
     * \param message The message.
     */
    FORCEINLINE static void LogException(const char* message)
    {
        m_instance->InvokeCallback(message, LogLevel::Fatal);
    }

    /**
    * \brief Writes exception message into log file.
    * \param format The message format.
    * \param args The message arguments.
    */
    FORCEINLINE static void LogException(const char* format, fmt::ArgList args)
    {
        auto string = fmt::format(format, args);
        auto cstring = string.c_str();
        m_instance->InvokeCallback(cstring, LogLevel::Fatal);
    }

    FMT_VARIADIC(static void, LogException, const char *)

    /**
    * \brief Writes error message into log file.
    * \param message The message.
    */
    FORCEINLINE static void LogError(const char* message)
    {
        m_instance->InvokeCallback(message, LogLevel::Error);
    }

    /**
    * \brief Writes error message into log file.
    * \param format The message format.
    * \param args The message arguments.
    */
    FORCEINLINE static void LogError(const char* format, fmt::ArgList args)
    {
        auto string = fmt::format(format, args);
        auto cstring = string.c_str();
        m_instance->InvokeCallback(cstring, LogLevel::Error);
    }

    FMT_VARIADIC(static void, LogError, const char *)

    /**
    * \brief Writes warning message into log file.
    * \param message The message.
    */
    FORCEINLINE static void LogWarning(const char* message)
    {
        m_instance->InvokeCallback(message, LogLevel::Warning);
    }

    /**
    * \brief Writes warning message into log file.
    * \param format The message format.
    * \param args The message arguments.
    */
    FORCEINLINE static void LogWarning(const char* format, fmt::ArgList args)
    {
        auto string = fmt::format(format, args);
        auto cstring = string.c_str();
        m_instance->InvokeCallback(cstring, LogLevel::Warning);
    }

    FMT_VARIADIC(static void, LogWarning, const char *)

    /**
    * \brief Writes info message into log file.
    * \param message The message.
    */
    FORCEINLINE static void LogInfo(const char* message)
    {
        m_instance->InvokeCallback(message, LogLevel::Message);
    }

    /**
    * \brief Writes info message into log file.
    * \param format The message format.
    * \param args The message arguments.
    */
    FORCEINLINE static void LogInfo(const char* format, fmt::ArgList args)
    {
        auto string = fmt::format(format, args);
        auto cstring = string.c_str();
        m_instance->InvokeCallback(cstring, LogLevel::Message);
    }

    FMT_VARIADIC(static void, LogInfo, const char *)

    /**
    * \brief Writes info message into log file.
    * \param message The message.
    */
    FORCEINLINE static void Log(const char* message)
    {
        m_instance->InvokeCallback(message, LogLevel::Message);
    }

    /**
    * \brief Writes info message into log file.
    * \param format The message format.
    * \param args The message arguments.
    */
    FORCEINLINE static void Log(const char* format, fmt::ArgList args)
    {
        auto string = fmt::format(format, args);
        auto cstring = string.c_str();
        m_instance->InvokeCallback(cstring, LogLevel::Message);
    }

    FMT_VARIADIC(static void, Log, const char *)
};

#endif // LOGGER_H
