// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

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

	// run the game
	GameMain core = {};
	core.run();

	return ERROR_SUCCESS;
}
#endif