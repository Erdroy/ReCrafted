// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Common/Platform/Platform.h"
#include "Common/Logger.h"
#include "Common/Guid.h"
#include "Common/List.h"

#ifdef _WIN32

#include <rpc.h>
#include <Windows.h>

void* Platform::m_currentWindow;
uint8_t Platform::m_cpuCount;
static Platform::EventDelegate m_eventDelegate;

std::thread::id g_mainThread;

static LARGE_INTEGER m_frequency;
static double m_start;
static double m_freqCoefficient;

List<Platform::EventDelegate> m_eventDelegates = {};

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
        RaiseException(0x406D1388, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)& info);
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

LRESULT CALLBACK WindowEventProcessor(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam)
{
    for(auto& eventDelegate : m_eventDelegates)
    {
        if (eventDelegate(hWnd, msg, wParam, lParam))
            return 0;
    }

    return m_eventDelegate(hWnd, msg, wParam, lParam);
}

void Platform::Initialize(const EventDelegate onEvent)
{
    m_eventDelegate = onEvent;

    // Set current thread as main
    g_mainThread = std::this_thread::get_id();

    // Initialize timer
    QueryPerformanceFrequency(&m_frequency);
    m_freqCoefficient = double(m_frequency.QuadPart) / 1000.0;

    m_start = GetMilliseconds();

    // Create window class
    WNDCLASSEX wnd;
    memset(&wnd, 0, sizeof(wnd));
    wnd.cbSize = sizeof(wnd);
    wnd.style = CS_HREDRAW | CS_VREDRAW;
    wnd.lpfnWndProc = WindowEventProcessor;
    wnd.hInstance = GetModuleHandle(nullptr);
    wnd.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wnd.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wnd.lpszClassName = L"recrafted";
    wnd.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    RegisterClassEx(&wnd);

    // Get cpu count
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    m_cpuCount = static_cast<uint8_t>(systemInfo.dwNumberOfProcessors);
}

void Platform::AddEventProcessor(const EventDelegate onEvent)
{
    m_eventDelegates.Add(onEvent);
}

void Platform::RemoveEventProcessor(const EventDelegate onEvent)
{
    m_eventDelegates.Remove(onEvent);
}

void Platform::Shutdown()
{
    // Unregister window class
    UnregisterClass(L"recrafted", GetModuleHandle(nullptr));
}

double Platform::GetMilliseconds()
{
    LARGE_INTEGER current;
    QueryPerformanceCounter(&current);
    return double(current.QuadPart) / m_freqCoefficient - m_start;
}

void Platform::Sleep(const uint32_t milliseconds)
{
    ::Sleep(milliseconds);
}

std::thread::id Platform::GetMainThreadId()
{
    return g_mainThread;
}

void Platform::ReportAssert(const String& expression, const String& fileName, const unsigned int line, const String& message)
{
    std::string formatted;
    if (message.StdStr().size() > 1)
    {
        formatted = fmt::format("Assertion failed! File: {0}:{1} - {2}\n Message: {3}", fileName.StdStr(), line, expression.StdStr(), message.StdStr());
    }
    else
    {
        formatted = fmt::format("Assertion failed! File: {0}:{1} - {2}", fileName.StdStr(), line, expression.StdStr());
    }

    Logger::Log(LogLevel::Assert, "{0}", formatted);

#if ASSERT_FAIL_SHOW_MESSAGE_BOX
    // Show message box
    MessageBoxA(static_cast<HWND>(m_currentWindow), formatted.c_str(), "Error", MB_OK | MB_ICONERROR);
#endif

#if _DEBUG && ASSERT_FAIL_DEBUG_BREAK
    __debugbreak();
#endif

#if ASSERT_FAIL_EXCEPTION
    throw std::exception(formatted.c_str());
#else
    // and exit...
    exit(-1);
#endif
}

void Platform::RunEvents()
{
    //Profiler::BeginProfile("RunEvents");
    MSG msg;
    msg.message = WM_NULL;
    while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    //Profiler::EndProfile();
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

void* Platform::CreateNewWindow(const String& windowName, const int width, const int height, uint64_t style)
{
    if (style == 0u)
        style = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MAXIMIZE;

    // create window now
    const auto window = CreateWindowW(L"recrafted", windowName.StdWStr().c_str(), static_cast<DWORD>(style), 0, 0, width, height
        , NULL, NULL, GetModuleHandle(nullptr), nullptr);

    // show the window
    ShowWindow(static_cast<HWND>(window), SW_MAXIMIZE);

    SetCurrentWindow(window);

    return window;
}

void Platform::MakeBorderLessWindow(void* windowHandle, const bool borderless)
{
    ASSERT(windowHandle);

    const auto hWnd = static_cast<HWND>(windowHandle);

    if (borderless)
    {
        const auto width = GetSystemMetrics(SM_CXSCREEN);
        const auto height = GetSystemMetrics(SM_CYSCREEN);
        SetWindowLongPtr(hWnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
        SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_FRAMECHANGED);
    }
    else
    {
        uint width;
        uint height;
        GetWindowSize(windowHandle, &width, &height);

        SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MAXIMIZE);
        SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_FRAMECHANGED);

        ShowWindow(hWnd, SW_MAXIMIZE);
    }
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

void Platform::SetCursorVisibility(const bool visible)
{
    ShowCursor(visible);
}

void Platform::GetWindowSize(void* windowHandle, unsigned* width, unsigned* height)
{
    RECT windowRect;
    GetClientRect(static_cast<HWND>(windowHandle), &windowRect);

    *width = windowRect.right - windowRect.left;
    *height = windowRect.bottom - windowRect.top;
}

void Platform::SetCursorPosition(const int x, const int y)
{
    POINT point = { long(x), long(y) };
    ClientToScreen(static_cast<HWND>(m_currentWindow), &point);
    SetCursorPos(point.x, point.y);
}

void Platform::SetThreadName(const char* name)
{
    ::SetThreadName(name);
}

int Platform::CpuCount()
{
    return static_cast<int>(m_cpuCount);
}

#endif
