// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "GameInfo.h"
#include "../Common/Text.h"

std::vector<Text> GameInfo::cliArguments;

bool GameInfo::containsArgument(Text text)
{
	for(auto i = 0u; i < cliArguments.size(); i ++)
	{
		return cliArguments[i] == text;
	}

	return false;
}

void GameInfo::parseArguments(Text text)
{
	auto length = text.length();
	auto isString = false;
	auto argStart = 0;

	for(auto i = 0; i < length; i ++)
	{
		auto ch = text[i];

		if(ch == static_cast<Char>('\"'))
		{
			if(isString)
			{
				// substr
				cliArguments.push_back(text.subtext(argStart, i - argStart));
				argStart = i + 2; // " and space
				i++;
				isString = false;
				continue;
			}

			isString = true;
			argStart = i + 1;
		}

		if(!isString)
		{
			if (ch == static_cast<Char>(' '))
			{
				// substr
				cliArguments.push_back(text.subtext(argStart, i - argStart));
				argStart = i + 1;
			}
		}

		if (ch == 0)
			break;
	}

	if(length == argStart)
		return;

	cliArguments.push_back(text.subtext(argStart, length - argStart));
}
