// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "GameMain.h"
#include "Scripting/ScriptingEngine.h"

void GameMain::initRuntime()
{
	API_FILE("Core/Game.Gen.cs");
	{
		API_COMMENT("Game class.");
		API_CLASS(PUBLIC, ABSTRACT, "ReCrafted.API.Core", "Game");
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

			API_COMMENT("Quits the game.");
			API_METHOD(PUBLIC, STATIC, "Quit", EXTERN);
			{
				API_BIND("ReCrafted.API.Core.Game::Quit", &GameMain::quit);
			}
			API_METHOD_END();

			API_COMMENT("The simulation tickrate. Default is 60");
			API_PROPERTY(PUBLIC, STATIC, "int", "Tickrate", GETSET);
			{
				API_BIND("ReCrafted.API.Core.Game::Internal_Tickrate_Get", &GameMain::getSimulationTickrate);
				API_BIND("ReCrafted.API.Core.Game::Internal_Tickrate_Set", &GameMain::setSimulationTickrate);
			}
			API_PROPERTY_END();
			
			API_COMMENT("Is game running state.");
			API_PROPERTY(PUBLIC, STATIC, "bool", "IsRunning", GET);
			{
				API_BIND("ReCrafted.API.Core.Game::Internal_IsRunning_Get", &GameMain::isRunning);
			}
			API_PROPERTY_END();
		}
		API_CLASS_END();
	}
	API_FILE_END();
}