// ReCrafted (c) 2016-2018 Always Too Late

#include "Bindings.h"
#include "Object.h"

#include "Common/Display.h"
#include "Common/Time.h"
#include "Common/Entity.h"
#include "Common/Profiler/Profiler.h"
#include "Common/Input/Input.h"
#include "Common/Input/KeyboardBuffer.h"
#include "Graphics/UI/UI.h"
#include "Graphics/Camera.h"
#include "Graphics/Font.h"
#include "Graphics/DebugDraw.h"
#include "Script.h"
#include "Core/Application.h"
#include "Graphics/WebUI/WebUI.h"
#include "Graphics/WebUI/WebUIView.h"

void Bindings::bind()
{
	Object::initRuntime();

    Application::initRuntime();

	Input::initRuntime();
	Time::initRuntime();
	Display::initRuntime();
	UI::initRuntime();
    WebUI::initRuntime();
    WebUIView::initRuntime();
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
