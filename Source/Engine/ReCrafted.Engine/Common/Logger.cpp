// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Logger.h"
#include "Common/IO/File.h"
#include "Common/IO/Directory.h"

Logger::Logger()
{
    if(!Directory::Exists("../Logs"))
        Directory::Create("../Logs");

    if (File::Exists("../Logs/log.txt"))
    {
        //File::Move("../Logs/log.txt", "../Logs/log_?.txt");
        // TODO: Backup old log
        File::Delete("../Logs/log.txt");
    }

    m_fileStream = new FileStream("../Logs/log.txt", File::OpenMode::BinaryWrite);
    m_logStream = new BinaryStream(*m_fileStream);
}

Logger::~Logger()
{
    delete m_logStream;

    m_fileStream->Dispose();
    delete m_fileStream;
}

void Logger::Flush() const
{
    // Flush file stream
    m_fileStream->Flush();
}

void Logger::WriteLog(const LogLevel level, const std::basic_string<char>& str)
{
    // Lock for whole logging
    // This is needed to maintain the order (time could differ).
    ScopeLock(m_lock);

    const auto time = Platform::GetMilliseconds() / 1000.0;

    const auto timeHours = Math::FloorToInt(time / 3600.0);
    const auto timeMinutes = Math::FloorToInt(time / 60.0);
    const auto timeSeconds = static_cast<int>(time - timeMinutes * 60);
    const auto timeMillis = static_cast<int>((time - Math::FloorToInt(time)) * 1000.0);

    // HHH:MM:SS.mmm
    const auto timeString = fmt::format("[{:0>3}:{:0>2}:{:0>2}.{:0>3}] ", timeHours, timeMinutes, timeSeconds, timeMillis);

    const char* typeString;
    switch (level) {
    case LogLevel::Message: typeString = "[Info] "; break;
    case LogLevel::Warning: typeString = "[Warning] "; break;
    case LogLevel::Assert: typeString = "[Assert] "; break;
    case LogLevel::Error: typeString = "[Error] "; break;
    case LogLevel::Fatal: typeString = "[FATAL] "; break;
    case LogLevel::Exception: typeString = "[Exception] "; break;
    default:
        typeString = "[Unknown] ";
    }

    auto logString = typeString + str + "\n";
    logString = timeString + logString;

    // Write log to the stream and flush
    m_logStream->WriteStringRaw(const_cast<char*>(logString.c_str()));
    Flush();
}

void Logger::WriteLog(const LogLevel level, const char* str)
{
    const auto string = std::basic_string<char>(str);
    GetInstance()->WriteLog(level, string);
}

void Logger::Initialize()
{
    // Setup the singleton instance, initialize logger
    // and write initial log
    Log(LogLevel::Message, "Hello, World!");
}

void Logger::Shutdown()
{
    // Flush and dispose
    GetInstance()->Flush();
    GetInstance()->Dispose();
}
