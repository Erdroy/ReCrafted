// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "GameMain.h"
#include "../Scripting/ScriptingEngine.h"

namespace Internal
{
	int TickrateGet()
	{
		return GameMain::getSimulationTickrate();
	}

	void TickrateSet(int value)
	{
		GameMain::setSimulationTickrate(value);
	}
}

void GameMain::initRuntime()
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

			API_COMMENT("Quits the game.");
			API_METHOD(PUBLIC, STATIC, "Quit", EXTERN);
			{
				API_BIND("ReCrafted.API.Core.Game::Quit", &GameMain::quit);
			}
			API_METHOD_END();

			API_COMMENT("The simulation tickrate. Default is 60");
			API_PROPERTY(PUBLIC, STATIC, "int", "Tickrate");
			{
				// this will create two methods: 
				// internal void Internal_TickrateSet(int value)
				// internal int Internal_TickrateGet()
				API_BIND("ReCrafted.API.Core.Game::Internal_TickrateGet", &Internal::TickrateGet);
				API_BIND("ReCrafted.API.Core.Game::Internal_TickrateSet", &Internal::TickrateSet);
			}
			API_PROPERTY_END();
		}
		API_CLASS_END();
	}
	API_FILE_END();
}