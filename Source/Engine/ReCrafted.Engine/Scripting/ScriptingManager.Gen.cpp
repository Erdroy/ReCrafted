// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "ScriptingManager.h"
#include "Common/Logger.h"
#include "Content/ContentManager.h"
#include "Content/Assets/Asset.h"
#include "Content/Assets/BinaryAsset.h"
#include "Content/Assets/JsonAsset.h"
#include "Core/Application.h"
#include "Core/Time.h"
#include "Core/Actors/ActorBase.h"
#include "Core/Actors/EmptyActor.h"
#include "Graphics/Texture.h"
#include "Input/InputManager.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Input/NullDevice.h"
#include "Scene/SceneManager.h"
#include "Scripting/Object.h"

void ScriptingManager::InitRuntime()
{
    Logger::InitRuntime();
    ContentManager::InitRuntime();
    Asset::InitRuntime();
    BinaryAsset::InitRuntime();
    JsonAsset::InitRuntime();
    Application::InitRuntime();
    Time::InitRuntime();
    ActorBase::InitRuntime();
    EmptyActor::InitRuntime();
    Texture::InitRuntime();
    InputManager::InitRuntime();
    Keyboard::InitRuntime();
    Mouse::InitRuntime();
    NullDevice::InitRuntime();
    SceneManager::InitRuntime();
    Object::InitRuntime();
}