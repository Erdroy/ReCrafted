// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "ScriptingEngine.h"
#include "../Core/Logger.h"
#include "../Common/Input.h"
#include "../Core/GameMain.h"

void ScriptingEngine::bind_all()
{
	Logger::initRuntime();
	Input::initRuntime();
	GameMain::initRuntime();
	Time::initRuntime();
}
