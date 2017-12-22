// ReCrafted © 2016-2017 Always Too Late

#pragma once

// ReCrafted prerequisites file

#ifndef RECRAFTED_H
#define RECRAFTED_H

#ifdef _WIN32
#include <Windows.h>
#endif

#include <ctime>
#include <vector>

#include "bgfxPrerequisites.h"

#include "Core/IDisposable.h"

#include "Core/Defines.h"
#include "Core/Types.h"
#include "Common/ReCraftedAPI.h"

#ifdef var
#	undef var
#endif
#define var auto
#define cvar const auto

#endif // RECRAFTED_H