// ReCrafted (c) 2016-2018 Always Too Late
#pragma once

#ifndef RENDERER_DEFINES_H
#define RENDERER_DEFINES_H

#include <iostream>
#include <stdint.h>

#define RENDERER_API 
#define RENDERER_FUNCTION(type) RENDERER_API type

#ifdef _WIN32
#	define RENDERER_ALIGN(bytes) __declspec(align(bytes))
#else
#	error Missing RENDERER_ALIGN implementation!
#endif

#define RENDERER_ENUM(name) \
inline constexpr name ::_enum operator|( name ::_enum a, name ::_enum b) {\
	return a = static_cast< name ::_enum> ((int)a | (int)b);\
}\
inline constexpr name ::_enum operator&( name ::_enum a, name ::_enum b) {\
	return a = static_cast< name ::_enum> ((int)a & (int)b);\
}

typedef unsigned char byte;

struct ObjectHandle
{
public:
	uint32_t idx = 0u;
};

#define RENDERER_DEFINE_HANDLE(name)                struct name##Handle : public ObjectHandle {  }

#define RENDERER_DEFINE_HANDLE_BEGIN(name)   struct name##Handle : public ObjectHandle { public:
#define RENDERER_DEFINE_HANDLE_END() \
}

#define RENDERER_DEFINE_HANDLE_ALLOCATOR(type, maxval) \
	type type##_table[maxval] = {}; \
	\
	type Alloc##type##() { \
		for(auto i = 1; i < maxval; i++) { \
			if( type##_table[i].idx == 0u ) { \
				type##_table[i].idx = i; \
				return type##_table[i]; } \
		} \
		return {}; \
	}  \
	\
	void Free##type##( type value ) { _ASSERT( value.idx != 0u ); type##_table[ value.idx ].idx = 0u; }

#define RENDERER_CHECK_HANDLE(handle) handle.idx != 0u

#define RENDERER_VALIDATE_HANDLE(name)\
        _ASSERT(RENDERER_CHECK_HANDLE(name));

#define RENDERER_FORCEINLINE __forceinline

#ifdef _DEBUG
#define CHECK_MAIN_THREAD() _ASSERT(std::this_thread::get_id() == g_mainThreadId)
#else
#define CHECK_MAIN_THREAD()
#endif

#endif // RENDERER_DEFINES_H
