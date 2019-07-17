// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

// Common/
template<typename TReturn, typename ... TArgs> struct Action;
template <class T> struct rc_allocator;
template <typename TType> struct Array;
template <typename TType> class ConcurrentQueue;
struct Lock;
template <typename ... TParams> class Event;
struct Guid;
template <typename TType> class List;
template <class TType> class Singleton;
class Logger;
class Memory;
template <typename TType> class Queue;
struct Signal;
struct String;

// Common/IO/
struct File;
class BinaryStream;
class Directory;
class FileStream;
class Stream;

// Common/Platform/
class Environment;
class Platform;

// Content/
class ContentManager;

// Content/Assets/
enum class AssetBaseType;
enum class AssetType : byte;
class Asset;
class BinaryAsset;
class JsonAsset;

// Core/
struct Transform;
class Application;
class ApplicationBase;
class ApplicationWindow;
class Display;
class MainLoop;
class Time;

// Core/Actors/
class ActorBase;
class EmptyActor;

// Core/SubSystems/
template <class TSubSystem> class SubSystem;
class SubSystemBase;
class SubSystemManager;

// Core/Threading
struct Task;
struct ITask;
class TaskManager;

// Game/
class GameManager;

// Physics/
enum class CharacterCollisionFlags;
enum class ForceMode : byte;
class Collider;
class BoxCollider;
class RigidBodyActor;
class DynamicRigidBodyActor;
class StaticRigidBodyActor;
class StepperTaskSimulate;
class StepperTask;
class MultiThreadStepper;
struct RayCastHit;
class PhysicsManager;
class PhysicsMaterial;
class PhysicsScene;
class ShapeCooker;

// Profiler/
class Profiler;

// Rendering/
enum class DrawMode : byte;
enum class MaterialFieldType : byte;
enum class RenderingComponentStage : byte;
class Camera;
class CameraActor;
class Mesh;
class RenderBuffer;
class RenderingBase;
class RenderingComponentBase;
template <class TRenderingComponent> class RenderingComponent;
class RenderingManager;
class Shader;
class Texture;

// Rendering/DeferredRendering
class DeferredRendering;

// Rendering/Debug/
class DebugDraw;

// Rendering/ImGui
class ImGuiManager;

// Rendering/Materials
class Material;
class MaterialField;

// Rendering/Models
class Model;

// Input/
enum class ActionType : sbyte;
enum class ActionEventType : sbyte;
enum class DeviceType : byte;
enum class InputType;
enum class KeyState : sbyte;
enum class Key : int;
enum class ButtonState : sbyte;
enum class Button : byte;
struct InputState;
struct InputData;
class ActionMap;
class InputDevice;
class InputManager;
class Keyboard;
class Mouse;
class NullDevice;

// Scene/
class SceneManager;

// Scripting/
struct Class;
struct Field;
struct Method;
class Assembly;
class Domain;
class Object;
class ObjectManager;
class Script;
class ScriptingManager;

// WebUI/
class WebUIManager;
class WebUIView;