// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include <fmt/chrono.h>

/// <summary>
///     The DateTime structure. Provides time formatting utility.
/// </summary>
struct DateTime
{
private:
    std::time_t m_time = 0;

public:
    /// <summary>
    ///     Formats this date into string using given format string.
    /// </summary>
    /// <param name="format">The format string.</param>
    /// <returns>The formatted date string.</returns>
    String Format(const String& format) const
    {
        return String::Format(format, *std::localtime(&m_time));
    }

public:
    /// <summary>
    ///     Returns DateTime structure with current date and time.
    /// </summary>
    /// <returns>The DateTime structure.</returns>
    static DateTime Now()
    {
        auto time = DateTime();
        time.m_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        return time;
    }
};
