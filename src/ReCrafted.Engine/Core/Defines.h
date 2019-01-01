// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef DEFINES_H
#define DEFINES_H

#include "Core/Assert.h"

#if _WIN32
#define getHInstance() static_cast<HINSTANCE>(GetModuleHandle(nullptr))
#endif

#ifndef FORCEINLINE
#define FORCEINLINE __forceinline
#endif

#define IS_MAIN_THREAD() Platform::GetMainThreadId() == std::this_thread::get_id()

#define SafeDispose(ptr) if(ptr) { ptr->Dispose(); ptr = nullptr;}
#define SafeDisposeNN(ptr) if(ptr) { ptr->Dispose(); }

#define SafeFree(ptr) if(ptr) { free(ptr); ptr = nullptr; }
#define SafeFreeNN(ptr) if(ptr) { free(ptr); }

#define SafeDelete(ptr) if(ptr) { delete ptr; ptr = nullptr; }
#define SafeDeleteNN(ptr) if(ptr) { delete ptr; }

#define SafeDeleteArray(ptr) if(ptr) { delete [] ptr; ptr = nullptr; }
#define SafeDeleteArrayNN(ptr) if(ptr) { delete [] ptr; }

#define MISSING_CODE(msg) Platform::ReportAssert(TEXT_CONST("MISSING_CODE()"), Text(__FILE__), (unsigned)(__LINE__), Text(msg))

#define DEFINE_ENUM(name) \
inline constexpr name operator|( name a, name b) {\
	return a = static_cast< name> ((int)a | (int)b);\
}\
inline constexpr name operator&( name a, name b) {\
	return a = static_cast< name> ((int)a & (int)b);\
}

#define PROPERTY(type, name)	\
	public: __inline type Get##name##() {		\
		return m_##name ;		\
	}							\
	public: __inline void Set##name##(type v) {	\
		m_##name = v;			\
	}							\
	private: type m_##name

#define PROPERTY_REF(type, name)	\
	public: __inline type& Get##name##() {		\
		return m_##name ;		\
	}							\
	public: __inline void Set##name##(type& v) {	\
		m_##name = v;			\
	}							\
	private: type m_##name

#define STATIC_PROPERTY(type, name)    \
    public: __inline static type Get##name##() {        \
        return m_##name ;        \
    }                            \
    public: __inline static void Set##name##(type v) {    \
        m_##name = v;            \
    }                            \
    private: static type m_##name

#define NULL_COPY_AND_ASSIGN(type) \
    type(const type& v) {(void)v;} \
    void operator=(const type& v) { (void)v; }

#ifdef _WIN32
#   define ALIGN(x) __declspec(align(x))
#else
#   define ALIGN(x) __attribute__ (aligned (x))
#endif

#endif // DEFINES_H
