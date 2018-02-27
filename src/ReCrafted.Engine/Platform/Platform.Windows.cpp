// ReCrafted (c) 2016-2018 Always Too Late

#include "Platform.h"
#include "ReCrafted.h"
#include "Common/Profiler/Profiler.h"
#include "Core/Logger.h"

#if _WIN32

#if USE_CRTDBG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <cstdio>
#include <Windows.h>

#pragma comment(lib, "Rpcrt4.lib")

static LARGE_INTEGER m_frequency;
static double m_start;
static double m_freqCoeff;

void* Platform::m_currentWindow;
unsigned char Platform::m_theadCount;
int Platform::m_cpuCount;

HICON m_currentCursor = nullptr;

HICON m_cursorArrow = nullptr;
HICON m_cursorIBeam = nullptr;

int m_currentCursorId = 0;

LRESULT CALLBACK WindowEventProcessor(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

void Platform::initialize()
{
	// initialize timer
	QueryPerformanceFrequency(&m_frequency);
	m_freqCoeff = double(m_frequency.QuadPart) / 1000.0;

	m_start = getMiliseconds();

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

    Logger::log("Initialized platform: Win32");
}

void Platform::shutdown()
{
#if USE_CRTDBG
    _CrtDumpMemoryLeaks();
#endif
}

Guid Platform::newGuid()
{
	Guid guid = {};
	UUID uuid = {};

	// Create UUID
	UuidCreate(&uuid);

	// Copy data to our Guid structure
	memcpy_s(&guid, sizeof Guid, &uuid, sizeof UUID);

	return guid;
}

double Platform::getMiliseconds()
{
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);
	return double(current.QuadPart) / m_freqCoeff - m_start;
}

int Platform::cpuCount()
{
	return m_cpuCount;
}

void* Platform::createWindow(Text windowName, int width, int height, const uint64_t style)
{
    var mStyle = style;

    if (mStyle == 0u)
        mStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MAXIMIZE;

    // create window now
    cvar window = CreateWindowW(L"recrafted", windowName.wstr().c_str(), static_cast<DWORD>(mStyle), 0, 0, width, height, NULL, NULL, getHInstance(), nullptr);

    // show the window
    ShowWindow(static_cast<HWND>(window), SW_MAXIMIZE); 

    setCurrentWindow(window);

    return window;
}

void Platform::runEvents()
{
    Profiler::beginProfile("RunEvents");
    MSG msg;
    msg.message = WM_NULL;
    while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    Profiler::endProfile();
}

void Platform::destroyWindow(void* windowHandle)
{
    // destroy window
    DestroyWindow(static_cast<HWND>(windowHandle));
}

void Platform::setCurrentWindow(void* windowHandle)
{
    m_currentWindow = windowHandle;
}

void* Platform::getCurrentWindow()
{
	return m_currentWindow;
}

int Platform::getCursorIcon()
{
    return m_currentCursorId;
}

void Platform::setCursorIcon(int iconId)
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

void Platform::getWindowSize(void* windowHandle, unsigned* width, unsigned* height)
{
    RECT windowRect;
    GetClientRect(static_cast<HWND>(windowHandle), &windowRect);

    *width = windowRect.right - windowRect.left;
    *height = windowRect.bottom - windowRect.top;
}

void Platform::setCursorPosition(uint16_t x, uint16_t y)
{
	POINT point = {};
	point.x = long(x);
	point.y = long(y);

	ClientToScreen(static_cast<HWND>(m_currentWindow), &point);
	SetCursorPos(point.x, point.y);
}

bool Platform::fileExists(const char* fileName)
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

void Platform::openFile(File* file, const char* fileName, OpenMode::_enum fileOpenMode)
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

void Platform::getWorkingDirectory(char* buffer)
{
	char i_buffer[MAX_PATH];
	GetModuleFileNameA(nullptr, i_buffer, MAX_PATH);

	// remove 'ReCrafted.exe', and copy
	auto length = strlen(i_buffer);
	for (auto i = length - 13u; i < length; i++)
	{
		i_buffer[i] = '\0';
	}

	// copy
	strcpy_s(buffer, length, i_buffer);
}
#endif
