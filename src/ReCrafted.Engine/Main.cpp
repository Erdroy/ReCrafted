// ReCrafted (c) 2016-2018 Always Too Late

#if defined(_WIN32)

// includes
#include "ReCraftedConfig.h"

// platform-specific includes
#include <Windows.h>

#include "ReCrafted.h"
#include "Core/GameInfo.h"
#include "Core/EngineMain.h"
#include "Platform/Platform.h"
#include "Graphics/WebUI/WebUIEngine.h"

#if RENDERER_TEST
void initRendererTests();
#endif

extern "C" { _declspec(dllexport) unsigned int NvOptimusEnablement = 0x00000001; }
extern "C" { __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1; }

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
	GameInfo::parseArguments(Text(GetCommandLineA()));

    // run WebUI engine children process
    WebUIEngine::runChildren();

    // create engine instance
    var engine = EngineMain();

    // initialize engine
    engine.initialize();

#if RENDERER_TEST
    initRendererTests();
#else
    // run engine loop
    engine.run();
#endif

    // shutdown engine
    engine.shutdown();

    // shutdown platform
    Platform::shutdown();

    rpmalloc_finalize();

	return ERROR_SUCCESS;
}
#endif
