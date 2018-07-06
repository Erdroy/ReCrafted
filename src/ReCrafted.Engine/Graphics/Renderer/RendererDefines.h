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

#define RENDERER_DEFINE_HANDLE(name)         struct name##Handle : public ObjectHandle {  }

#define RENDERER_DEFINE_HANDLE_WITH_DESCRIPTOR(type) \
struct type##Handle : public ObjectHandle {  };\
struct type##Description; \
type##Description& Get##type##Description(type##Handle handle); \
struct type##Description { public:

#define RENDERER_DEFINE_HANDLE_WITH_DESCRIPTOR_END() }

#define RENDERER_DEFINE_HANDLE_DESCRIPTOR_TABLE(type, maxval) \
    type##Description type##_desc_table[maxval] = {};\
    type##Description& Get##type##Description(type##Handle handle) {\
        ASSERT( handle.idx != 0u );\
        return type##_desc_table[handle.idx];\
    }

#define RENDERER_DEFINE_HANDLE_ALLOCATOR(type, maxval)  \
    Lock type##lock = {};                               \
	type type##_table[maxval] = {};                     \
	type Alloc##type##() {                              \
        ScopeLock(type##lock);                          \
		for(auto i = 1; i < maxval; i++) {              \
			if( type##_table[i].idx == 0u ) {           \
				type##_table[i].idx = i;                \
				return type##_table[i]; }               \
		} return {};                                    \
	}                                                   \
	void Free##type##( type value ) {                   \
        ScopeLock(type##lock);                          \
        ASSERT( value.idx != 0u );                     \
        type##_table[ value.idx ] = {};                 \
    }

#define RENDERER_CHECK_HANDLE(handle) handle.idx != 0u

#define RENDERER_VALIDATE_HANDLE(name)\
        ASSERT(RENDERER_CHECK_HANDLE(name));

#define RENDERER_FORCEINLINE __forceinline

#ifdef _DEBUG
#define CHECK_MAIN_THREAD() ASSERT(std::this_thread::get_id() == g_mainThreadId)
#else
#define CHECK_MAIN_THREAD() true
#endif

#endif // RENDERER_DEFINES_H
