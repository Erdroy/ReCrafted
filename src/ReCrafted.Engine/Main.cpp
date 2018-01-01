// ReCrafted (c) 2016-2018 Always Too Late

#include "Core/GameInfo.h"
#include "Graphics/HTML5_UI/HTML5_UI.h"
#ifdef _WIN32

// includes
#include <Windows.h>
#include "Core/GameMain.h"

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

	//HTML5UI::initChildren();

	// run the game
	GameMain core = {};
	core.run();

	return ERROR_SUCCESS;
}
#endif