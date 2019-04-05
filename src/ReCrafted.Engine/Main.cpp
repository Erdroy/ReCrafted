// ReCrafted (c) 2016-2019 Always Too Late

#include "Core/Logger.h"
#if defined(_WIN32)

// includes
#include "ReCraftedConfig.h"

#include "ReCrafted.h"
#include "Core/GameInfo.h"
#include "Core/EngineMain.h"
#include "Platform/Platform.h"

#include <Windows.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

extern "C" {
    __declspec(dllexport) unsigned int NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

/// <summary>
/// WinMain - Main entry for Windows platform
/// </summary>
int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
#if DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    // Initialize memory allocator TODO: This should be called from static constructor, as soon as possible!
    Memory::Initialize(Memory::AllocatorType::OS);
    
    // parse arguments
    var arguments = Text(GetCommandLineA());
    GameInfo::ParseArguments(arguments);

    // create engine instance
    var engine = EngineMain();
    
    // initialize engine
    engine.Initialize();

    // run engine loop
    engine.Run();

    // Shutdown engine
    engine.Shutdown();

#if DEBUG
    // Dump memory leaks
    _CrtDumpMemoryLeaks();
#endif

    // Shutdown platform
    Platform::Shutdown();

    return ERROR_SUCCESS;
}
#endif
