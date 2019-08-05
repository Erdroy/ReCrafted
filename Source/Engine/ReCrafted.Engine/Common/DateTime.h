// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include <fmt/chrono.h>

struct DateTime
{
private:
    std::time_t m_time = 0;

public:
    String Format(const String& format) const
    {
        return String::Format(format, *std::localtime(&m_time));
    }

public:
    static DateTime Now()
    {
        auto time = DateTime();
        time.m_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        return time;
    }
};