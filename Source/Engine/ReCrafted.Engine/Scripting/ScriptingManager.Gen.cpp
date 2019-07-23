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
#include "Physics/CharacterActor.h"
#include "Physics/DynamicRigidBodyActor.h"
#include "Physics/PhysicsManager.h"
#include "Physics/PhysicsMaterial.h"
#include "Physics/PhysicsScene.h"
#include "Physics/RigidBodyActor.h"
#include "Physics/StaticRigidBodyActor.h"
#include "Physics/Colliders/BoxCollider.h"
#include "Physics/Colliders/Collider.h"
#include "Physics/Colliders/MeshCollider.h"
#include "Physics/Colliders/SphereCollider.h"
#include "Profiler/Profiler.h"
#include "Rendering/Camera.h"
#include "Rendering/CameraActor.h"
#include "Rendering/Mesh.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Rendering/Debug/DebugDraw.h"
#include "Rendering/Materials/Material.h"
#include "Rendering/Models/Model.h"
#include "Scene/SceneManager.h"
#include "Scripting/Object.h"
#include "Scripting/Script.h"
#include "Voxels/VoxelObjectBase.h"
#include "Voxels/VoxelObjectManager.h"
#include "Voxels/VoxelPlanet.h"
#include "Voxels/Assets/VoxelObjectAsset.h"
#include "WebUI/WebUIManager.h"
#include "WebUI/WebUIView.h"

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
    CharacterActor::InitRuntime();
    DynamicRigidBodyActor::InitRuntime();
    PhysicsManager::InitRuntime();
    PhysicsMaterial::InitRuntime();
    PhysicsScene::InitRuntime();
    RigidBodyActor::InitRuntime();
    StaticRigidBodyActor::InitRuntime();
    BoxCollider::InitRuntime();
    Collider::InitRuntime();
    MeshCollider::InitRuntime();
    SphereCollider::InitRuntime();
    Profiler::InitRuntime();
    Camera::InitRuntime();
    CameraActor::InitRuntime();
    Mesh::InitRuntime();
    Shader::InitRuntime();
    Texture::InitRuntime();
    DebugDraw::InitRuntime();
    Material::InitRuntime();
    Model::InitRuntime();
    SceneManager::InitRuntime();
    Object::InitRuntime();
    Script::InitRuntime();
    VoxelObjectBase::InitRuntime();
    VoxelObjectManager::InitRuntime();
    VoxelPlanet::InitRuntime();
    VoxelObjectAsset::InitRuntime();
    WebUIManager::InitRuntime();
    WebUIView::InitRuntime();
}