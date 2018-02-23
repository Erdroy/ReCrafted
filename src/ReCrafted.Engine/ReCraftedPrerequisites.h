// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef RECRAFTEDPREREQUISITES_H
#define RECRAFTEDPREREQUISITES_H

#include <ctime>
#include <vector>

#include "Core/Defines.h"
#include "Core/Types.h"
#include "Core/Logger.h"
#include "Common/ReCraftedAPI.h"

#ifdef var
#	undef var
#endif
#define var auto
#define cvar const auto

interface IDisposable;
interface IResource;

class Application;
class ApplicationBase;
class ApplicationWindow;

class UpdateLoop;

class EngineMain;
class EngineComponent;
class EngineComponentManager;

struct Delegate;

struct Vector2;
struct Vector3;
struct Vector4;
struct Matrix;

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
