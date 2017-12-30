// ReCrafted © 2016-2017 Always Too Late

#include "Bindings.h"
#include "Object.h"

#include "Core/GameMain.h"
#include "Common/Display.h"
#include "Common/Profiler/Profiler.h"
#include "Common/Input/Input.h"
#include "Common/Input/KeyboardBuffer.h"
#include "Graphics/UI/UI.h"
#include "Graphics/Camera.h"
#include "Graphics/Font.h"
#include "Graphics/DebugDraw.h"

void Bindings::bind()
{
	Object::initRuntime();

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
	DebugDraw::initRuntime();
	Profiler::initRuntime();
	KeyboardBuffer::initRuntime();
}

void Bindings::shutdown()
{

}
