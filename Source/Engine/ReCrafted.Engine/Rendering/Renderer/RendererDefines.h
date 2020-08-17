// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
#pragma once

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

struct EmptyDescription
{
    
};

struct ObjectHandle
{
public:
    uint32_t idx = 0u;
};

#define RENDERER_DEFINE_HANDLE(name)         struct name##Handle : public ObjectHandle {  }

#define RENDERER_DEFINE_HANDLE_WITH_DESCRIPTOR(type) \
struct type##Handle : public ObjectHandle {  };\
struct type##Description { public:

#define RENDERER_DEFINE_HANDLE_WITH_DESCRIPTOR_END() }

#define RENDERER_CHECK_HANDLE(handle) handle.idx != 0u

#define RENDERER_VALIDATE_HANDLE(name)\
        ASSERT(RENDERER_CHECK_HANDLE(name));

#define RENDERER_FORCEINLINE __forceinline

#ifdef _DEBUG
#define CHECK_MAIN_THREAD() ASSERT(std::this_thread::get_id() == g_mainThreadId)
#else
#define CHECK_MAIN_THREAD() true
#endif
