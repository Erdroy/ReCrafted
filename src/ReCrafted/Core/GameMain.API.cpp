// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "GameMain.h"

namespace Internal
{
	
}

void Logger::initRuntime()
{
	API_FILE("Core/Game.Gen.cs");
	{
		API_COMMENT("Game class.");
		API_CLASS(PUBLIC, ABSTRACT, "ReCrafted.API.Core", "Game", PARTIAL);
		{
			API_METHOD(PROTECTED, ABSTRACT, "Initialize");
			API_METHOD_END();

			API_METHOD(PROTECTED, ABSTRACT, "Update");
			API_METHOD_END();

			API_METHOD(PROTECTED, ABSTRACT, "Simulate");
			API_METHOD_END();

			API_METHOD(PROTECTED, ABSTRACT, "Draw");
			API_METHOD_END();

			API_METHOD(PROTECTED, ABSTRACT, "DrawUI");
			API_METHOD_END();

			API_METHOD(PROTECTED, ABSTRACT, "Shutdown");
			API_METHOD_END();
		}
		API_CLASS_END();
	}
	API_FILE_END();
}