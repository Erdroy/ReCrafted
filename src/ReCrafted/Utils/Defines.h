// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef DEFINES_H
#define DEFINES_H

#if _WIN32
#define getHInstance() static_cast<HINSTANCE>(GetModuleHandle(nullptr))
#endif

#ifndef FORCEINLINE
#define FORCEINLINE __inline
#endif

#define SafeDispose(ptr) if(ptr) { ptr->dispose(); ptr = nullptr;}

#ifndef VS_LOG_H
#define VS_LOG_H

#if defined(_WIN32)
#include <Windows.h>

inline void LOG(const char* text)
{
#if defined(_DEBUG)
	OutputDebugStringA(text);
	OutputDebugStringA("\n");
#endif
}
#endif

#ifdef _DEBUG
#define VS_LOG(text) LOG(text)
#else
#define VS_LOG(text)
#endif

#endif // VS_LOG_H

#endif // DEFINES_H