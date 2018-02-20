// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef RECRAFTEDPREREQUISITES_H
#define RECRAFTEDPREREQUISITES_H

#ifdef _WIN32
// TODO: remove this...
#include <Windows.h>
#endif

#include <ctime>
#include <vector>

#include "bgfxPrerequisites.h"

#include "Core/IDisposable.h"
#include "Core/Defines.h"
#include "Core/Types.h"
#include "Core/Logger.h"
#include "Common/ReCraftedAPI.h"

#ifdef var
#	undef var
#endif
#define var auto
#define cvar const auto

class IDisposable;

class ApplicationBase;
class ApplicationWindow;

class UpdateLoop;

class EngineMain;
class EngineComponent;
class EngineComponentManager;

class Profiler;
class Logger;
class Display;
class Time;
class Input;
class PhysicsManager;
class Rendering;
class UI;
class EntityPool;
class Universe;

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
