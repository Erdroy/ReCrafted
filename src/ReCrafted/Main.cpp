// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

// define type of build
#define GAME
#define SERVER

#ifdef _WIN32
#define PLATFORM Windows

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


#ifndef PLATFORM
	#define PLATFORM Unknown
#endif