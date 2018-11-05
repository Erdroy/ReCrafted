// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef RECRAFTEDPREREQUISITES_H
#define RECRAFTEDPREREQUISITES_H

#include <ctime>
#include <vector>
#include <thread>

#include "Core/Defines.h"
#include "Core/Types.h"
#include "Core/Memory.h"
#include "Core/IDisposable.h"
#include "Core/Singleton.h"
#include "Common/ReCraftedAPI.h"

#include "Platform/Platform.h"

#ifdef var
#	undef var
#endif
#define var auto
#define cvar const auto
#define rvar var&
#define crvar cvar&

struct IDisposable;
struct IResource;

class Application;
class ApplicationBase;
class ApplicationWindow;

class UpdateLoop;

class EngineMain;
class EngineComponentBase;
class EngineComponentManager;

struct Text;

class Profiler;
class Display;
class Time;
class Input;
class PhysicsSystem;
class Graphics;
class UI;
class EntityPool;
class Universe;

class WebUI;
class WebUIView;
class WebUIEngine;

class TaskManager;
struct Task;

class ScriptingEngine;
class Bindings;
class Domain;
class Assembly;
class Class;
class Field;
class Method;
class Object;
class Script;

class KeyboardBuffer;

#endif // RECRAFTEDPREREQUISITES_H
