// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Logger.h"

Logger::Logger()
{
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

void Logger::Log(const char* string)
{
    GetInstance()->m_logStream->WriteStringRaw(const_cast<char*>(string));
}

void Logger::Initialize()
{
    // TODO: Backup old log file

    GetInstance()->m_fileStream = new FileStream("log.txt", File::OpenMode::BinaryWrite);
    GetInstance()->m_logStream = new BinaryStream(*GetInstance()->m_fileStream);
}

void Logger::Shutdown()
{
    // Flush and dispose
    GetInstance()->Flush();
    GetInstance()->Dispose();
}
