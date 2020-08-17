// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

class Platform final
{
public:
    using EventDelegate = uint64_t(*)(void*, uint32_t, uint64_t, uint64_t);

private:
    static void* m_currentWindow;
    static uint8_t m_cpuCount;

public:
    /// <summary>
    ///     Initializes platform implementation.
    /// </summary>
    static void Initialize(EventDelegate onEvent);

    /// <summary>
    ///     Adds event processor delegate to internal list.
    /// </summary>
    static void AddEventProcessor(EventDelegate onEvent);

    /// <summary>
    ///     Removes event processor delegate from internal list.
    /// </summary>
    static void RemoveEventProcessor(EventDelegate onEvent);

    /// <summary>
    ///     Shutdowns platform implementation.
    /// </summary>
    static void Shutdown();

    /// <summary>
    ///     Get time in milliseconds.
    /// </summary>
    static double GetMilliseconds();

    /// <summary>
    ///     Sleeps the current thread for given time.
    /// </summary>
    /// <param name="milliseconds">The time to sleep.</param>
    static void Sleep(uint32_t milliseconds);

    /// <summary>
    ///     Gets the main thread id.
    /// </summary>
    /// <returns>The main thread id.</returns>
    static std::thread::id GetMainThreadId();

    /// <summary>
    ///     Reports failed assert message.
    /// </summary>
    /// <param name="expression">The assert expression.</param>
    /// <param name="fileName">The source file.</param>
    /// <param name="line">The source file line where the assert failed.</param>
    /// <param name="message">The additional message.</param>
    static void ReportAssert(const String& expression, const String& fileName, unsigned int line, const String& message);

    /// <summary>
    ///     Runs all application events.
    /// </summary>
    static void RunEvents();

    /// <summary>
    ///     Creates new Guid.
    /// </summary>
    /// <returns>The new unique guid.</returns>
    static Guid NewGuid();

    /// <summary>
    ///     Creates new window.
    /// </summary>
    /// <param name="windowName">The window title.</param>
    /// <param name="width">The initial window client-size width.</param>
    /// <param name="height">The initial window client-size height.</param>
    /// <param name="style">The style of the window. Look for docs of all of the platforms.</param>
    /// <returns>The created window handle.</returns>
    static void* CreateNewWindow(const String& windowName, int width = 1280, int height = 720, uint64_t style = 0u);

    /// <summary>
    ///     Sets given window as borderless window.
    /// </summary>
    /// <param name="windowHandle">The target window that will be set as borderless.</param>
    /// <param name="borderless">When true, window will become borderless, otherwise just a normal window.</param>
    static void MakeBorderLessWindow(void* windowHandle, bool borderless);

    /// <summary>
    ///     Destroys window with given handle.
    /// </summary>
    /// <param name="windowHandle">Handle of the window.</param>
    static void DestroyWindow(void* windowHandle);

    /// <summary>
    ///     Sets given window handle as current (main).
    /// </summary>
    /// <param name="windowHandle">Handle of the window.</param>
    static void SetCurrentWindow(void* windowHandle);

    /// <summary>
    ///     Gets the main game window handle.
    /// </summary>
    /// <returns>The game window handle.</returns>
    static void* GetCurrentWindow();

    /// <summary>
    ///     Get size of game window.
    /// </summary>
    /// <param name="width">(out)The width.</param>
    /// <param name="height">(out)The height.</param>
    static void GetCurrentWindowSize(unsigned int* width, unsigned int* height)
    {
        return GetWindowSize(m_currentWindow, width, height);
    }

    /// <summary>
    ///     Show or hide cursor.
    /// </summary>
    static void SetCursorVisibility(bool visible);

    /// <summary>
    ///     Get size of given window.
    /// </summary>
    /// <param name="windowHandle">The window handle.</param>
    /// <param name="width">(out)The width.</param>
    /// <param name="height">(out)The height.</param>
    static void GetWindowSize(void* windowHandle, unsigned int* width, unsigned int* height);

    /// <summary>
    ///     Set the cursor position.
    /// </summary>
    /// <param name="x">The x coordinate of target cursor position in pixels.</param>
    /// <param name="y">The y coordinate of target cursor position in pixels.</param>
    static void SetCursorPosition(int x, int y);

    /// <summary>
    ///     Sets debug name for the current thread.
    /// </summary>
    /// <param name="name">The desired thread name.</param>
    static void SetThreadName(const char* name);

    /// <summary>
    ///     Gets the CPU count.
    /// </summary>
    static int CpuCount();
};
