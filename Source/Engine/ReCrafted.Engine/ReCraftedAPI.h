// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#define API_USING(ns)

/// <summary>
///     Defines custom type translation. Overrides all API's generator internal 
///     type translations by the name of the specified one.
/// </summary>
#define API_CUSTOM_TRANSLATION(nativeType, managedTypeName)

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
    static const char* Namespace();

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

/// <summary>
///     Only valid for properties. Forces the generator to use by-value data passing.
/// </summary>
#define byvalue

/// <summary>
///     Only valid for properties. Forces the generator to remove 'Get' and/or 'Set' prefixes from the function name.
/// </summary>
#define noprefix

/// <summary>
///     Valid for properties and functions. Forces the generator to allow function calls from non-main threads.
/// </summary>
/// <remarks>
///     Calling thread should be registered for scripting backend, otherwise calling this function will result in undefined behavior.
/// </remarks>
#define multithread

/// <summary>
///     Valid for classes. Defines custom namespace. Example: API_CLASS(..., customNamespace="ReCrafted.API").
/// </summary>
#define customNamespace

/// <summary>
///     Valid for classes. Marks the class as it can be possibly generic. This adds additional object creation functionality.
/// </summary>
/// <remarks>Cannot be used on abstract classes!</remarks>
#define generic
