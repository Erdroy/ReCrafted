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
#include "Script.h"
#include "Core/Application.h"
#include "Graphics/WebUI/WebUI.h"
#include "Graphics/WebUI/WebUIView.h"
#include "Graphics/DebugDraw.h"

void Bindings::Bind()
{
    Object::InitRuntime();

    Application::InitRuntime();

    Input::InitRuntime();
    Time::InitRuntime();
    Display::InitRuntime();
    UI::InitRuntime();
    WebUI::InitRuntime();
    WebUIView::InitRuntime();
    Camera::InitRuntime();
    Texture2D::InitRuntime();
    DebugDraw::InitRuntime();
    Font::InitRuntime();
    Entity::InitRuntime();
    Script::InitRuntime();
    Profiler::InitRuntime();
    KeyboardBuffer::InitRuntime();
}

void Bindings::Shutdown()
{
}
