// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Bindings.h"
#include "Common/Input.h"
#include "Common/Display.h"
#include "Core/GameMain.h"
#include "Core/Logger.h"
#include "Graphics/UI/UI.h"
#include "Object.h"

void Bindings::bind()
{
	Logger::initRuntime();
	Input::initRuntime();
	GameMain::initRuntime();
	Time::initRuntime();
	Display::initRuntime();
	UI::initRuntime();
	Object::initRuntime();
}
