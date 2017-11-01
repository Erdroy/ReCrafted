// ReCrafted © 2016-2017 Always Too Late

#include "Bindings.h"
#include "Object.h"

#include "Common/Input.h"
#include "Common/Display.h"
#include "Core/GameMain.h"
#include "Core/Logger.h"
#include "Graphics/UI/UI.h"
#include "Graphics/Camera.h"
#include "Graphics/Font.h"

void Bindings::bind()
{
	Object::initRuntime();

	Logger::initRuntime();
	Input::initRuntime();
	GameMain::initRuntime();
	Time::initRuntime();
	Display::initRuntime();
	UI::initRuntime();
	Camera::initRuntime();
	Texture2D::initRuntime();
	Font::initRuntime();
    Entity::initRuntime();
    Script::initRuntime();
}

void Bindings::shutdown()
{

}
