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

void Logger::Log(const char* string)
{
    GetInstance()->m_logStream->WriteStringRaw(const_cast<char*>(string));
}

void Logger::Initialize()
{
    // Setup the singleton instance, initialize logger
    // and write initial log
    Log("Hello, World!");
}

void Logger::Shutdown()
{
    // Flush and dispose
    GetInstance()->Flush();
    GetInstance()->Dispose();
}
