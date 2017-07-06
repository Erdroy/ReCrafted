// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef GAMEINFO_H
#define GAMEINFO_H

// includes
#include <vector>

struct Text;

class GameInfo
{
public:
	/// <summary>
	/// Checks if specified argument is present.
	/// </summary>
	static bool containsArgument(Text text);

	/// <summary>
	/// Parses argument string and adds to the cliArguments array.
	/// </summary>
	static void parseArguments(Text text);

public:
	/// <summary>
	/// The command line arguments passed when the application was launched.
	/// </summary>
	static std::vector<Text> cliArguments;
};

#endif // GAMEINFO_H
