// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#define API_USING(ns)

#define API_CLASS(...)
#define API_STRUCT(...)

#define API_FUNCTION(...)
#define API_PROPERTY(...)

#define API_CLASS_BODY()            \
    friend class ScriptingManager;  \
    private:                        \
    static void InitRuntime();

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