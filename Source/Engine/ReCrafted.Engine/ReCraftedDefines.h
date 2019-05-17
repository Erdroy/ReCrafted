// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#ifdef RC_API
# undef RC_API
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 
#define FORCE_INLINE __forceinline
#define FORCE_BREAKPOINT() __debugbreak()
#else
#define FORCE_INLINE
#define FORCE_BREAKPOINT()
#endif

#define DEFINE_ENUM(name)                               \
inline constexpr name operator|( name a, name b) {      \
	return a = static_cast< name> ((int)a | (int)b);    \
}                                                       \
inline constexpr name operator&( name a, name b) {      \
	return a = static_cast< name> ((int)a & (int)b);    \
}

#ifdef _WIN32
#   define ALIGN(x) __declspec(align(x))
#else
#   define ALIGN(x) __attribute__ (aligned (x))
#endif

#if !defined(DELETE_CTOR_COPY) && !defined(DELETE_CTOR_MOVE) && !defined(DELETE_CTOR_COPY_MOVE)
#define DELETE_CTOR_COPY(className) className(const className& other) = delete;
#define DELETE_CTOR_MOVE(className) className(const className&& other) = delete;
#define DELETE_CTOR_COPY_MOVE(className)		\
	DELETE_CTOR_COPY(className)				    \
	DELETE_CTOR_MOVE(className)
#endif

#if !defined(DELETE_OPERATOR_COPY) && !defined(DELETE_OPERATOR_MOVE) && !defined(DELETE_OPERATOR_COPY_MOVE)
#define DELETE_OPERATOR_COPY(className) void operator=(className& other) = delete;
#define DELETE_OPERATOR_MOVE(className) void operator=(className&& other) = delete;
#define DELETE_OPERATOR_COPY_MOVE(className)	\
	DELETE_OPERATOR_COPY(className)			    \
	DELETE_OPERATOR_MOVE(className)
#endif

#if !defined(DELETE_COPY_MOVE)
#define DELETE_COPY_MOVE(className)				\
	DELETE_CTOR_COPY_MOVE(className)			\
	DELETE_OPERATOR_COPY_MOVE(className)
#endif
