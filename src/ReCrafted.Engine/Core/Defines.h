// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef DEFINES_H
#define DEFINES_H

#if _WIN32
#define getHInstance() static_cast<HINSTANCE>(GetModuleHandle(nullptr))
#endif

#ifndef FORCEINLINE
#define FORCEINLINE __forceinline
#endif

#ifndef FORCE_NO_INLINE
#define FORCE_NO_INLINE __declspec(noinline)
#endif

#define SafeDispose(ptr) if(ptr) { ptr->dispose(); ptr = nullptr;}
#define SafeDisposeNN(ptr) if(ptr) { ptr->dispose(); }

#define SafeFree(ptr) if(ptr) { free(ptr); ptr = nullptr; }
#define SafeFreeNN(ptr) if(ptr) { free(ptr); }

#define SafeDelete(ptr) if(ptr) { delete ptr; ptr = nullptr; }
#define SafeDeleteNN(ptr) if(ptr) { delete ptr; }

#define SafeDeleteArray(ptr) if(ptr) { delete [] ptr; ptr = nullptr; }
#define SafeDeleteArrayNN(ptr) if(ptr) { delete [] ptr; }

#ifndef VS_LOG_H
#define VS_LOG_H

#if defined(_WIN32)
#include <Windows.h>

inline void LOG(const char* text)
{
	//OutputDebugStringA(text);
	//OutputDebugStringA("\n");
}
#endif

#define VS_LOG(text) LOG(text)

#endif // VS_LOG_H

#define MISSING_CODE() throw "Code fragment is missing"

#define PROPERTY(type, name)	\
	public: __inline type get_##name##() {		\
		return m_##name ;		\
	}							\
	public: __inline void set_##name##(type v) {	\
		m_##name = v;			\
	}							\
	private: type m_##name
#define STATIC_PROPERTY(type, name)    \
    public: __inline static type get_##name##() {        \
        return m_##name ;        \
    }                            \
    public: __inline static void set_##name##(type v) {    \
        m_##name = v;            \
    }                            \
    private: static type m_##name

#define ALIGN(x) __declspec(align(x)) 

#ifdef interface
#	undef interface
#endif

#ifdef _WIN32
//#	define interface __interface
#	define interface struct
#else
#	define interface struct
#endif

#endif // DEFINES_H