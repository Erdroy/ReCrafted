// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "ScriptingEngine.h"
#include "../Core/Logger.h"
#include "../Core/Input.h"

void ScriptingEngine::bind_all()
{
	Logger::initRuntime();
	Input::initRuntime();
}
