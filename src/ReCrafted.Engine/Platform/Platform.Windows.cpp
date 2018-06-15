// ReCrafted (c) 2016-2018 Always Too Late

#include <thread>
#if _WIN32

#if USE_CRTDBG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <cstdio>
#include <Windows.h>

#include "Platform.h"
#include "ReCrafted.h"
#include "Common/Profiler/Profiler.h"
#include "Core/Logger.h"

static LARGE_INTEGER m_frequency;
static double m_start;
static double m_freqCoeff;

void* Platform::m_currentWindow;
unsigned char Platform::m_theadCount;
int Platform::m_cpuCount;
std::thread::id Platform::m_mainThread;

HICON m_currentCursor = nullptr;

HICON m_cursorArrow = nullptr;
HICON m_cursorIBeam = nullptr;

int m_currentCursorId = 0;

LRESULT CALLBACK WindowEventProcessor(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

// source: https://stackoverflow.com/questions/10121560/stdthread-naming-your-thread
#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
    DWORD dwType; // Must be 0x1000.
    LPCSTR szName; // Pointer to name (in user addr space).
    DWORD dwThreadID; // Thread ID (-1=caller thread).
    DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

void SetThreadName(uint32_t dwThreadID, const char* threadName)
{
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = threadName;
    info.dwThreadID = dwThreadID;
    info.dwFlags = 0;

    __try
    {
        RaiseException(0x406D1388, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
    }
}

void SetThreadName(const char* threadName)
{
    SetThreadName(GetCurrentThreadId(), threadName);
}

void SetThreadName(std::thread* thread, const char* threadName)
{
    DWORD threadId = ::GetThreadId(static_cast<HANDLE>(thread->native_handle()));
    SetThreadName(threadId, threadName);
}

void Platform::Initialize()
{
    // set current thread as main
    m_mainThread = std::this_thread::get_id();

    // initialize timer
    QueryPerformanceFrequency(&m_frequency);
    m_freqCoeff = double(m_frequency.QuadPart) / 1000.0;

    m_start = GetMiliseconds();

    // get cpu count
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    m_cpuCount = sysinfo.dwNumberOfProcessors;

    // create cursor
    m_currentCursor = LoadCursor(nullptr, IDC_ARROW);

    m_cursorArrow = m_currentCursor;
    m_cursorIBeam = LoadCursor(nullptr, IDC_IBEAM);

    m_currentCursorId = 0;

    // create window class
    cvar instance = getHInstance();

    WNDCLASSEX wnd;
    memset(&wnd, 0, sizeof(wnd));
    wnd.cbSize = sizeof(wnd);
    wnd.style = CS_HREDRAW | CS_VREDRAW;
    wnd.lpfnWndProc = WindowEventProcessor;
    wnd.hInstance = instance;
    wnd.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wnd.hCursor = m_currentCursor;
    wnd.lpszClassName = L"recrafted";
    wnd.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    RegisterClassEx(&wnd);

    Logger::Log("Initialized platform: Win32");
}

void Platform::Shutdown()
{
#if USE_CRTDBG
    _CrtDumpMemoryLeaks();
#endif
}

Guid Platform::NewGuid()
{
    Guid guid = {};
    UUID uuid = {};

    // Create UUID
    UuidCreate(&uuid);

    // Copy data to our Guid structure
    memcpy_s(&guid, sizeof Guid, &uuid, sizeof UUID);

    return guid;
}

double Platform::GetMiliseconds()
{
    LARGE_INTEGER current;
    QueryPerformanceCounter(&current);
    return double(current.QuadPart) / m_freqCoeff - m_start;
}

int Platform::CpuCount()
{
    return m_cpuCount;
}

void Platform::Log(const char* logMessage)
{
    OutputDebugStringA(logMessage);
    OutputDebugStringA("\n");
}

void* Platform::CreateNewWindow(Text& windowName, int width, int height, const uint64_t style)
{
    var mStyle = style;

    if (mStyle == 0u)
        mStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MAXIMIZE;

    // create window now
    cvar window = CreateWindowW(L"recrafted", windowName.StdWStr().c_str(), static_cast<DWORD>(mStyle), 0, 0, width, height
            , NULL, NULL, getHInstance(), nullptr);

    // show the window
    ShowWindow(static_cast<HWND>(window), SW_MAXIMIZE);

    SetCurrentWindow(window);

    return window;
}

void Platform::RunEvents()
{
    Profiler::BeginProfile("RunEvents");
    MSG msg;
    msg.message = WM_NULL;
    while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    Profiler::EndProfile();
}

void Platform::DestroyWindow(void* windowHandle)
{
    // Destroy window
    ::DestroyWindow(static_cast<HWND>(windowHandle));
}

void Platform::SetCurrentWindow(void* windowHandle)
{
    m_currentWindow = windowHandle;
}

void* Platform::GetCurrentWindow()
{
    return m_currentWindow;
}

int Platform::GetCursorIcon()
{
    return m_currentCursorId;
}

void Platform::SetCursorIcon(int iconId)
{
    switch (iconId)
    {
    case 1:
        m_currentCursor = m_cursorIBeam;
        break;
    default:
        m_currentCursor = m_cursorArrow;
        break;
    }

    m_currentCursorId = iconId;
}

void Platform::GetWindowSize(void* windowHandle, unsigned* width, unsigned* height)
{
    RECT windowRect;
    GetClientRect(static_cast<HWND>(windowHandle), &windowRect);

    *width = windowRect.right - windowRect.left;
    *height = windowRect.bottom - windowRect.top;
}

void Platform::Sleep(unsigned int miliseconds)
{
    ::Sleep(miliseconds);
}

void Platform::SetCursorPosition(uint16_t x, uint16_t y)
{
    POINT point = {};
    point.x = long(x);
    point.y = long(y);

    ClientToScreen(static_cast<HWND>(m_currentWindow), &point);
    SetCursorPos(point.x, point.y);
}

bool Platform::FileExists(const char* fileName)
{
    FILE* file;

    // check if file exists 
    // by just trying to open for read 
    // and checking the error
    cvar error = fopen_s(&file, fileName, "r");

    // close the file now if exist to avoid some leaks or somethin
    if (error == 0)
        fclose(file);

    // return the value
    return error == 0;
}

void Platform::OpenFile(File* file, const char* fileName, OpenMode::_enum fileOpenMode)
{
    strcpy_s(file->FileName, fileName);

    // select open mode
    const char* openMode = "??";
    switch (fileOpenMode)
    {
    case OpenMode::OpenRead:
        openMode = "r";
        break;
    case OpenMode::OpenWrite:
        openMode = "w";
        break;
    case OpenMode::OpenAppend:
        openMode = "w+";
        break;
    case OpenMode::OpenReadWrite:
        openMode = "r+";
        break;
    default: break;
    }

    file->m_file = fopen(fileName, openMode);

    // file not found
    assert(file->m_file != nullptr);

    cvar filep = static_cast<FILE*>(file->m_file);

    // measure file size
    fseek(filep, 0, SEEK_END);
    file->FileSize = ftell(filep);
    fseek(filep, 0, SEEK_SET);
}

void Platform::GetWorkingDirectory(char* buffer)
{
    char i_buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, i_buffer, MAX_PATH);

    // Remove 'ReCrafted.exe', and copy
    auto length = strlen(i_buffer);
    for (auto i = length - 13u; i < length; i++)
    {
        i_buffer[i] = '\0';
    }

    // copy
    strcpy_s(buffer, length, i_buffer);
}

void Platform::SetThreadName(const char* name)
{
    ::SetThreadName(name);
}

void Platform::ReportAssert(Text expression, Text fileName, unsigned int line, Text message)
{
    std::string formated;
    if(message.StdStr().size() > 1)
    {
        formated = fmt::format("Assertion failed! File: {0}:{1} - {2}\n Message: {3}", fileName.StdStr(), line, expression.StdStr(), message.StdStr());
    }
    else
    {
        formated = fmt::format("Assertion failed! File: {0}:{1} - {2}", fileName.StdStr(), line, expression.StdStr());
    }

    Logger::LogException("{0}", formated);

    // Show message box
    MessageBoxA(static_cast<HWND>(m_currentWindow), formated.c_str(), "Error", MB_OK | MB_ICONERROR);

#if _DEBUG
    __debugbreak();
#endif

    // and exit...
    exit(-1);
}

#endif
