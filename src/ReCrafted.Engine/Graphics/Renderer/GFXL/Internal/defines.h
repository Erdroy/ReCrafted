// GFXL - Graphics Library (c) 2016-2017 Damian 'Erdroy' Korczowski
#pragma once

#ifndef DEFINES_H
#define DEFINES_H

#include <iostream>
#include <stdint.h>

#define GFXL_API 
#define GFXL_FUNCTION(type) GFXL_API type

#ifdef _WIN32
#	define GFXL_ALIGN(bytes) __declspec(align(bytes))
#else
#	error Missing GFXL_ALIGN implementation!
#endif

#define GFXL_ENUM(name) \
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

#define GFXL_DEFINE_HANDLE(name)                struct name##Handle : public ObjectHandle {  }

#define GFXL_DEFINE_HANDLE_BEGIN(name)   struct name##Handle : public ObjectHandle { public:
#define GFXL_DEFINE_HANDLE_END() \
}

#define GFXL_DEFINE_HANDLE_ALLOCATOR(type, maxval) \
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

#define GFXL_CHECK_HANDLE(handle) handle.idx == 0u

#define GFXL_VALIDATE_HANDLE(name)\
	if (GFXL_CHECK_HANDLE(##name##)) Internal::Fatal("Invalid handle passed!");

#define GFXL_FORCEINLINE __forceinline

#endif // DEFINES_H
