// ReCrafted © 2016-2017 Always Too Late

#include "Platform.Windows.h"

#pragma comment(lib, "Rpcrt4.lib")

HWND Platform::m_gameWindow;
LARGE_INTEGER Platform::m_frequency;
double Platform::m_start;
double Platform::m_freqCoeff;
HANDLE Platform::m_threadPool[MAX_THREADS];
unsigned char Platform::m_theadCount;
std::vector<Platform::QueueEntry> Platform::m_queue;

DWORD WINAPI ThreadWorker(LPVOID lpParam)
{

	return 0;
}