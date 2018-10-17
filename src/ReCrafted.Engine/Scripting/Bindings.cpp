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
#include "Audio/AudioSound.h"
#include "Voxels/Assets/VoxelMaterial.h"

static spp::sparse_hash_map<std::string, Action<Object*>> g_objectMap;

void Bindings::Bind()
{
    g_objectMap = {};
    g_objectMap.clear();

    Object::InitRuntime();

    // == Engine components ==
    Application::InitRuntime();
    Input::InitRuntime();
    Time::InitRuntime();
    Display::InitRuntime();
    UI::InitRuntime();
    DebugDraw::InitRuntime();
    Font::InitRuntime();
    Profiler::InitRuntime();
    KeyboardBuffer::InitRuntime();


    // == ECS ==
    ComponentSystem::InitRuntime();
    Entity::InitRuntime();
    World::InitRuntime();


    // == WebUI ==
    WebUI::InitRuntime();
    WebUIView::InitRuntime();


    // == Voxels ==
    VoxelMaterialManager::InitRuntime();


    // == Spawnable objects ==
    Camera::InitRuntime();

    // == Content ==
    ContentManager::InitRuntime();

    // == Assets ==
    Texture::InitRuntime();
    VoxelMaterial::InitRuntime();
    AudioSound::InitRuntime();
    Shader::InitRuntime();


}

void Bindings::Shutdown()
{
}

void Bindings::RegisterObject(const char* fullName, const Action<Object*>& createFunction)
{
    g_objectMap.insert(std::make_pair(fullName, createFunction));
}

Action<Object*>* Bindings::GetObjectCreator(const char* fullName)
{
    cvar it = g_objectMap.find(fullName);

    if (it == g_objectMap.end())
        return nullptr;

    return &it->second;
}
