// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include "Common/List.h"
#include "Common/String.h"

/// <summary>
///     Environment class. Provides basic application environment data (command-line arguments, os version etc.). 
/// </summary>
class Environment
{
private:
    static List<String> m_arguments;

public:
    /// <summary>
    ///     Parses given string and sets the command-line arguments.
    /// </summary>
    /// <param name="string">The argument-containing string.</param>
    static void SetCommandLine(const String& string);

    /// <summary>
    ///     Gets list of command-line arguments produced by SetCommandLine.
    /// </summary>
    /// <returns>The list.</returns>
    static const List<String>& GetCommandLineArguments();

    /// <summary>
    ///     Returns true when given argument exists in the current command line args.
    /// </summary>
    static bool HasCommandLineArgument(const String& argument);

    /// <summary>
    ///     Returns true whenever current build is built for debugging
    /// </summary>
    /// <returns>True when debug settings are included.</returns>
    static bool IsDebugBuild();

    /// <summary>
    ///     Returns true whenever current build is built for release.
    /// </summary>
    /// <returns>True when release settings are included.</returns>
    static bool IsReleaseBuild();
};
