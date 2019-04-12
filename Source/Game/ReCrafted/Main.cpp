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

/**
 * \brief WinMain - Main entry for Windows platform
 */
int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
#if DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    {
        // Initialize memory allocator TODO: This should be called from static constructor, as soon as possible!
        Memory::Initialize(Memory::AllocatorType::OS);

        Application app;
        app.Run();
    }
#if DEBUG
    // Dump memory leaks
    if (_CrtDumpMemoryLeaks() == 0)
        OutputDebugStringA("NO MEMORY LEAKS FOUND");
#endif
    return ERROR_SUCCESS;
}
#endif

// TODO: Linux/OSX entry points
