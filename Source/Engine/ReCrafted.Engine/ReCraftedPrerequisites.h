// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

// Common/
template<typename TReturn, typename ... TArgs> struct Action;
template <class T> struct rc_allocator;
struct Lock;
struct String;
template <typename ... TParams> class Event;
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

// Input/
enum class ActionType;
enum class ActionEventType;
enum class DeviceType;
enum class InputType;
enum class KeyState;
enum class Key;
enum class ButtonState;
enum class Button;
struct InputState;
struct InputData;
class ActionMap;
class InputDevice;
class InputManager;
class Keyboard;
class Mouse;
class NullDevice;

// Scripting/
struct Class;
class Assembly;
class Domain;
class Object;
class ScriptingManager;
