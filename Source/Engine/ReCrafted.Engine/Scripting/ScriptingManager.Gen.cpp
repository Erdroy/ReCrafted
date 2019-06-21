// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "ScriptingManager.h"
#include "Common/Logger.h"
#include "Content/ContentManager.h"
#include "Content/Assets/Asset.h"
#include "Content/Assets/BinaryAsset.h"
#include "Content/Assets/JsonAsset.h"
#include "Core/Application.h"
#include "Core/Display.h"
#include "Core/Time.h"
#include "Core/Actors/ActorBase.h"
#include "Core/Actors/EmptyActor.h"
#include "Input/InputManager.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Input/NullDevice.h"
#include "Rendering/Camera.h"
#include "Rendering/CameraActor.h"
#include "Rendering/Material.h"
#include "Rendering/Model.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Rendering/Debug/DebugDraw.h"
#include "Scene/SceneManager.h"
#include "Scripting/Object.h"
#include "Scripting/Script.h"

void ScriptingManager::InitRuntime()
{
    Logger::InitRuntime();
    ContentManager::InitRuntime();
    Asset::InitRuntime();
    BinaryAsset::InitRuntime();
    JsonAsset::InitRuntime();
    Application::InitRuntime();
    Display::InitRuntime();
    Time::InitRuntime();
    ActorBase::InitRuntime();
    EmptyActor::InitRuntime();
    InputManager::InitRuntime();
    Keyboard::InitRuntime();
    Mouse::InitRuntime();
    NullDevice::InitRuntime();
    Camera::InitRuntime();
    CameraActor::InitRuntime();
    Material::InitRuntime();
    Model::InitRuntime();
    Shader::InitRuntime();
    Texture::InitRuntime();
    DebugDraw::InitRuntime();
    SceneManager::InitRuntime();
    Object::InitRuntime();
    Script::InitRuntime();
}