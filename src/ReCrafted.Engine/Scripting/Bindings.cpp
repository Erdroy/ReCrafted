// ReCrafted (c) 2016-2018 Always Too Late

#include "Bindings.h"
#include "Object.h"

#include "Audio/AudioSound.h"
#include "Common/ActorBase.h"
#include "Common/EmptyActor.h"
#include "Common/Display.h"
#include "Common/Time.h"
#include "Common/Entities/ComponentSystem.h"
#include "Common/Entities/World.h"
#include "Common/Entities/Entity.h"
#include "Common/Profiler/Profiler.h"
#include "Common/Input/Input.h"
#include "Common/Input/KeyboardBuffer.h"
#include "Core/Application.h"
#include "Graphics/Camera.h"
#include "Graphics/DebugDraw.h"
#include "Graphics/Font.h"
#include "Physics/Collision.h"
#include "Physics/CharacterActor.h"
#include "Physics/PhysicsManager.h"
#include "Physics/RigidBodyActor.h"
#include "UI/UI.h"
#include "WebUI/WebUI.h"
#include "WebUI/WebUIView.h"
#include "Voxels/VoxelMaterialManager.h"
#include "Voxels/Assets/VoxelMaterial.h"

static spp::sparse_hash_map<std::string, Action<Object*, bool>> g_objectMap;

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


    // == Scripting ==
    Script::InitRuntime();


    // == Physics ==
    Collision::InitRuntime();
    PhysicsManager::InitRuntime();


    // == Actors ==
    ActorBase::InitRuntime(); // base
    EmptyActor::InitRuntime();
    RigidBodyActor::InitRuntime();
    CharacterActor::InitRuntime();


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

void Bindings::RegisterObject(const char* fullName, const Action<Object*, bool>& createFunction)
{
    g_objectMap.insert(std::make_pair(fullName, createFunction));
}

Action<Object*, bool>* Bindings::GetObjectCreator(const char* fullName)
{
    cvar it = g_objectMap.find(fullName);

    if (it == g_objectMap.end())
        return nullptr;

    return &it->second;
}
