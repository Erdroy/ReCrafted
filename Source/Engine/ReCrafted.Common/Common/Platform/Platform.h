// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

class Platform final
{
public:
    using EventDelegate = uint64_t(*)(void*, uint32_t, uint64_t, uint64_t);

private:
    static void* m_currentWindow;

public:
    /**
     * \brief Initializes platform.
     */
    static void Initialize(EventDelegate onEvent);

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
    static void ReportAssert(const String& expression, const String& fileName, unsigned int line, const String& message);

    /**
     * \brief Runs all application events.
     */
    static void RunEvents();

    /**
     * \brief Creates new window.
     * \param windowName The window title.
     * \param width The initial window client-size width.
     * \param height The initial window client-size height.
     * \param style The style of the window. Look for docs of all of the platforms.
     * \return The created window handle.
     */
    static void* CreateNewWindow(const String& windowName, int width = 1280, int height = 720, uint64_t style = 0u);

    /**
     * \brief Sets given window as borderless window.
     * \param windowHandle The target window that will be set as borderless.
     * \param borderless When true, window will become borderless, otherwise just a normal window.
     */
    static void MakeBorderLessWindow(void* windowHandle, bool borderless);

    /**
     * \brief Destroys window with given handle.
     * \param windowHandle Handle of the window.
     */
    static void DestroyWindow(void* windowHandle);

    /**
     * \brief Sets given window handle as current (main).
     * \param windowHandle Handle of the window.
     */
    static void SetCurrentWindow(void* windowHandle);

    /**
    * \brief Gets the main game window handle.
    * \return The game window handle.
    */
    static void* GetCurrentWindow();

    /**
     * \brief Get size of game window.
     * \param width (out)The width.
     * \param height (out)The height.
     */
    static void GetCurrentWindowSize(unsigned int* width, unsigned int* height)
    {
        return GetWindowSize(m_currentWindow, width, height);
    }

    /**
     * \brief Show or hide cursor.
     */
    static void SetCursorVisibility(bool visible);

    /**
    * \brief Get size of given window.
    * \param windowHandle The window handle.
    * \param width (out)The width.
    * \param height (out)The height.
    */
    static void GetWindowSize(void* windowHandle, unsigned int* width, unsigned int* height);

    /**
     * \brief Set the cursor position.
     * \param x The x coordinate of target cursor position in pixels.
     * \param y The y coordinate of target cursor position in pixels.
     */
    static void SetCursorPosition(int x, int y);

    /**
     * \brief Sets debug name for the current thread.
     * \param name The desired thread name.
     */
    static void SetThreadName(const char* name);
};
