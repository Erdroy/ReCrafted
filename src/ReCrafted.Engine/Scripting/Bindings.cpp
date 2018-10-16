// ReCrafted (c) 2016-2018 Always Too Late

#include "Bindings.h"
#include "Object.h"

#include "Common/Display.h"
#include "Common/Time.h"
#include "Common/Entities/ComponentSystem.h"
#include "Common/Entities/World.h"
#include "Common/Entities/Entity.h"
#include "Common/Profiler/Profiler.h"
#include "Common/Input/Input.h"
#include "Common/Input/KeyboardBuffer.h"
#include "UI/UI.h"
#include "Graphics/Camera.h"
#include "Graphics/Font.h"
#include "Core/Application.h"
#include "WebUI/WebUI.h"
#include "WebUI/WebUIView.h"
#include "Graphics/DebugDraw.h"
#include "Voxels/VoxelMaterialManager.h"

void Bindings::Bind()
{
    Object::InitRuntime();

    Application::InitRuntime();

    Input::InitRuntime();
    Time::InitRuntime();
    Display::InitRuntime();
    UI::InitRuntime();
    ComponentSystem::InitRuntime();
    Entity::InitRuntime();
    World::InitRuntime();
    WebUI::InitRuntime();
    WebUIView::InitRuntime();
    Camera::InitRuntime();
    Texture::InitRuntime();
    DebugDraw::InitRuntime();
    Font::InitRuntime();
    Profiler::InitRuntime();
    KeyboardBuffer::InitRuntime();

    VoxelMaterialManager::InitRuntime();
}

void Bindings::Shutdown()
{
}
