// ReCrafted (c) 2016-2018 Always Too Late

// includes
#include "ReCrafted.h"
#include "Core/GameInfo.h"
#include "Core/EngineMain.h"

#ifdef _WIN32

// platform-specific includes
#include <Windows.h>

/// <summary>
/// WinMain - Main entry for Windows platform
/// </summary>
int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	GameInfo::parseArguments(Text(GetCommandLineA()));

    // create engine instance
    var engine = EngineMain();

    // initialize engine
    engine.initialize();

    // run engine loop
    engine.run();

    // shutdown engine
    engine.shutdown();

	return ERROR_SUCCESS;
}
#endif