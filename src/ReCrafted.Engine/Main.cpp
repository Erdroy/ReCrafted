// ReCrafted (c) 2016-2018 Always Too Late

#if defined(_WIN32)

// includes
#include "ReCraftedConfig.h"

#include "ReCrafted.h"
#include "Core/GameInfo.h"
#include "Core/EngineMain.h"
#include "Platform/Platform.h"
#include "Graphics/WebUI/WebUIEngine.h"

// platform-specific includes
#include <Windows.h>

extern "C" {
_declspec(dllexport) unsigned int NvOptimusEnablement = 0x00000001;
}

extern "C" {
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
    // initialize memory
    rpmalloc_initialize();

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

    // Shutdown platform
    Platform::Shutdown();

    rpmalloc_finalize();

    return ERROR_SUCCESS;
}
#endif
