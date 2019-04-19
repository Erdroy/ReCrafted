// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include <ReCrafted.h>
#include <Core/Application.h>

#ifdef _WIN32

#include <Windows.h>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

extern "C" {
    __declspec(dllexport) unsigned int NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

class Entry
{
public:
    Entry()
    {
        // Note: This constructor should be called at the very beginning

#if DEBUG
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

        // Initialize memory allocator
        Memory::Initialize(Memory::AllocatorType::OS);
    }

    ~Entry()
    {
#if DEBUG
        // Dump memory leaks
        if (_CrtDumpMemoryLeaks() == 0)
            OutputDebugStringA("NO MEMORY LEAKS FOUND\n");
        else
        {
            OutputDebugStringA("FOUND MEMORY LEAKS\n");
            FORCE_BREAKPOINT();
        }
#endif
    }
};

static Entry EntryInstance;

/**
 * \brief WinMain - Main entry for Windows platform
 */
int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    Application app;
    app.Run();
    return ERROR_SUCCESS;
}
#endif

// TODO: Linux/OSX entry points
