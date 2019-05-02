// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#define API_USING(ns)

#define API_CLASS(...)
#define API_STRUCT(...)

#define API_FUNCTION(...)
#define API_PROPERTY(...)

#define API_CLASS_BODY()            \
    friend class Object;            \
    friend class APIProxy;          \
    friend class ScriptingManager;  \
    private:                        \
    static void InitRuntime();      \
    static const char* Fullname();  \
    static const char* Name();      \
    static const char* Namespace(); \

#define API_BIND(name, method)      \
    mono_add_internal_call(name, (const void*)(method))

#define MONO_STRING_TO_CSTR(str)    \
    mono_string_to_utf8(str)

#define MONO_STRING_TO_STR(str)     \
    String((Char*)mono_string_chars(str))

#define MONO_FREE(ptr)              \
    mono_free(ptr);

#define MONO_FREE_STUB(value)

/// <summary>
///     Marks class to skip it's inheritance.
/// </summary>
#define noinherit

/// <summary>
///     Works the same as extern. Skips generation of proxy function in C# API file.
/// </summary>
/// <remarks>Function name will get additional prefix 'Internal', eg.: GetTime -> InternalGetTime.</remarks>
/// <remarks>Only static function can skip it's proxy.</remarks>
#define noproxy
