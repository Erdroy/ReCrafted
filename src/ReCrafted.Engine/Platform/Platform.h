// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PLATFORM_H
#define PLATFORM_H

// includes
#include "Core/Guid.h"
#include <thread>

#define PATHLENGTH 512

struct OpenMode
{
    enum _enum
    {
        OpenRead,
        OpenWrite,
        OpenAppend,

        OpenReadWrite
    };
};

struct File
{
    friend class Platform;

private:
    void* m_file = nullptr;

public:
    char FileName[PATHLENGTH];
    size_t FileSize = 0u;
    bool AutoClose = true;

    ~File();

    void Seek(long position) const;

    void Read(void* buffer, size_t length, size_t offset = 0) const;

    void Read(void* buffer) const;

    void Write(void* data, const size_t dataLenght) const;

    void Flush() const;

    void Close() const;
};

class Platform
{
private:
    static void* m_currentWindow;

    // timer
    static unsigned char m_theadCount;
    static int m_cpuCount;

    static std::thread::id m_mainThread;

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
     * \brief Creates new Guid.
     * \return The new unique guid.
     */
    static Guid NewGuid();

    /**
    * \brief Get time in miliseconds.
    * \return The time in miliseconds.
    */
    static double GetMiliseconds();

    /**
     * \brief Reports failed assert message.
     * \param expression The assert expression.
     * \param fileName The source file.
     * \param line The source file line where the assert failed.
     * \param message The additional message.
     */
    static void ReportAssert(Text expression, Text fileName, unsigned int line, Text message);

    /**
     * \brief Gets the CPU count.
     * \return The CPU count. 
     */
    static int CpuCount();

    /**
     * \brief Standard logging. Used to output messages to Visual Studio.
     * \param logMessage The log message.
     */
    static void Log(const char* logMessage);

    /**
     * \brief Runs all system loop events.
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
    static void* CreateNewWindow(Text& windowName, int width = 1280, int height = 720, uint64_t style = 0u);

    static void MakeBorderLessWindow(void* windowHandle, bool borderLess);

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
     * \brief Gets cursor icon id.
     * \return The cursor icon id.
     */
    static int GetCursorIcon();

    /**
     * \brief Sets cursor icon id.
     * \param iconId The cursor icon id.
     */
    static void SetCursorIcon(int iconId);

    /**
    * \brief Get size of given window.
    * \param width (out)The width.
    * \param height (out)The height.
    */
    static void GetWindowSize(void* windowHandle, unsigned int* width, unsigned int* height);

    /**
     * \brief Sleeps the current thread for given time.
     * \param miliseconds The time to sleep.
     */
    static void Sleep(unsigned int miliseconds);

    /**
     * \brief Set the cursor position.
     * \param x The x coord of target cursor position in pixels.
     * \param y The y coord of target cursor position in pixels.
     */
    static void SetCursorPosition(uint16_t x, uint16_t y);

    /**
     * \brief Checks if file exists.
     * \param fileName The file path and name. Using working directory.
     * \return True when file exists.
     */
    static bool FileExists(const char* fileName);

    /**
     * \brief Open file for operations.
     * Make sure that the file exists.
     * \param file The file path and name. Using working directory.
     * \param fileName The open mode.
     * \param fileOpenMode The file.
     */
    static void OpenFile(File* file, const char* fileName, OpenMode::_enum fileOpenMode = OpenMode::OpenReadWrite);

    /**
     * \brief Get the current working directory.
     * \param buffer Output buffer.
     */
    static void GetWorkingDirectory(char* buffer);

    /**
     * \brief Sets debug name for the current thread.
     * \param name The desired thread name.
     */
    static void SetThreadName(const char* name);

    /**
     * \brief Gets the main thread id.
     * \return The main thread id.
     */
    static std::thread::id GetMainThreadId()
    {
        return m_mainThread;
    }
};

#endif // PLATFORM_H
