// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

// Common/
template<typename TReturn, typename ... TArgs> struct Action;
template <class T> struct rc_allocator;
struct Lock;
struct String;
template <typename ... TParams> class Event;
struct Guid;
template <typename TType> class List;
template <class TType> class Singleton;
class Logger;
class Memory;

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
template<typename TActor> class Actor;
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

// Graphics/

// Renderer/

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
class ScriptingManager;
