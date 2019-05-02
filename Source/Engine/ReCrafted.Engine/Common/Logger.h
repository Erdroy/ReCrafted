// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/Singleton.h"
#include "Common/IO/FileStream.h"
#include "Common/IO/BinaryStream.h"
#include "Common/Lock.h"

#include <fmt/format.h>

enum class LogLevel
{
    Message,
    Warning,
    Error,

    Assert,
    Fatal,
};

API_CLASS(public, static, partial, noinherit)
class Logger final : public Singleton<Logger>
{
    API_CLASS_BODY()
    DELETE_COPY_MOVE(Logger)
    friend class Application;

private:
    FileStream* m_fileStream = nullptr;
    BinaryStream* m_logStream = nullptr;
    Lock m_lock;

public:
    Logger();
    ~Logger();

private:
    void Flush() const;

public:
    void WriteLog(LogLevel level, const std::basic_string<char>& str);

public:
    template<typename... TArgs>
    static void Log(const LogLevel level, const char* format, const TArgs& ... args)
    {
        GetInstance()->WriteLog(level, fmt::vformat(format, fmt::make_format_args(args...)));
    }

    template<typename... TArgs>
    static void Log(const char* format, const TArgs& ... args)
    {
        Log<TArgs...>(LogLevel::Message, format, args...);
    }

    template<typename... TArgs>
    static void LogWarning(const char* format, const TArgs& ... args)
    {
        Log<TArgs...>(LogLevel::Warning, format, args...);
    }

    template<typename... TArgs>
    static void LogError(const char* format, const TArgs& ... args)
    {
        Log<TArgs...>(LogLevel::Error, format, args...);
    }

private:
    API_FUNCTION(noproxy)
    static void WriteLog(LogLevel level, const char* str);

protected:
    static void Initialize();
    static void Shutdown();
};
