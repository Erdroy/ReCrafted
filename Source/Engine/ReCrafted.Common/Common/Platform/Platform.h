// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

class Platform final
{
public:
    /**
     * \brief Initializes platform.
     */
    static void Initialize();

    /**
     * \brief Shutdowns platform.
     */
    static void Shutdown();

    /**
    * \brief Get time in milliseconds.
    * \return The time in milliseconds.
    */
    static double GetMilliseconds();

    /**
     * \brief Sleeps the current thread for given time.
     * \param milliseconds The time to sleep.
     */
    static void Sleep(uint32_t milliseconds);

    /**
     * \brief Gets the main thread id.
     * \return The main thread id.
     */
    static std::thread::id GetMainThreadId();

    /**
     * \brief Reports failed assert message.
     * \param expression The assert expression.
     * \param fileName The source file.
     * \param line The source file line where the assert failed.
     * \param message The additional message.
     */
    static void ReportAssert(String expression, String fileName, unsigned int line, String message);
};
