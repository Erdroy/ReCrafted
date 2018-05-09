// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef GAMEINFO_H
#define GAMEINFO_H

// includes
#include "Core/Containers/Array.h"

struct Text;

class GameInfo
{
public:
	/// <summary>
	/// Checks if specified argument is present.
	/// </summary>
	static bool containsArgument(Text& text);

	/// <summary>
	/// Parses argument string and adds to the cliArguments array.
	/// </summary>
	static void parseArguments(Text& text);

public:
	/// <summary>
	/// The command line arguments passed when the application was launched.
	/// </summary>
	static Array<Text> cliArguments;
};

#endif // GAMEINFO_H
