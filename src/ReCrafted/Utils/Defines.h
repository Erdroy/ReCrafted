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

#ifndef FORCE_NO_INLINE
#define FORCE_NO_INLINE __declspec(noinline)
#endif

#define SafeDispose(ptr) if(ptr) { ptr->dispose(); ptr = nullptr;}
#define SafeDisposeNN(ptr) if(ptr) { ptr->dispose(); }

#ifndef VS_LOG_H
#define VS_LOG_H

#if defined(_WIN32)
#include <Windows.h>

inline void LOG(const char* text)
{
	OutputDebugStringA(text);
	OutputDebugStringA("\n");
}
#endif

#define VS_LOG(text) LOG(text)

#endif // VS_LOG_H

#define MISSING_CODE() throw "Code fragment is missing"

#define Property(type, name)	\
	type m_##name;				\
	__inline type get##name##() {		\
		return m_##name ;		\
	}							\
	__inline void set##name##(type v) {	\
		m_##name = v;			\
	}							\

#endif // DEFINES_H