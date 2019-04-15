// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/Singleton.h"
#include "Common/IO/FileStream.h"
#include "Common/IO/BinaryStream.h"

class Logger final : public Singleton<Logger>
{
    DELETE_COPY_MOVE(Logger)
    friend class Application;

private:
    FileStream* m_fileStream = nullptr;
    BinaryStream* m_logStream = nullptr;

public:
    Logger();
    ~Logger();

private:
    void Flush() const;

public:
    static void Log(const char* string);

protected:
    static void Initialize();
    static void Shutdown();
};
