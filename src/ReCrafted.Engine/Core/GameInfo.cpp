// ReCrafted (c) 2016-2018 Always Too Late

#include "GameInfo.h"
#include "Common/Text.h"
#include "Platform/Platform.h"

Array<Text> GameInfo::cliArguments;

bool GameInfo::ContainsArgument(const Text& text)
{
    for (auto i = 0u; i < cliArguments.Size(); i ++)
    {
        if (cliArguments[i] == text)
            return true;
    }

    return false;
}

void GameInfo::ParseArguments(const Text& text)
{
    auto length = text.Length();
    auto isString = false;
    auto argStart = 0;

    for (auto i = 0; i < length; i ++)
    {
        auto ch = text[i];

        if (ch == static_cast<Char>('\"'))
        {
            if (isString)
            {
                // substr
                cliArguments.Add(text.Subtext(argStart, i - argStart));
                argStart = i + 2; // " and space
                i++;
                isString = false;
                continue;
            }

            isString = true;
            argStart = i + 1;
        }

        if (!isString)
        {
            if (ch == static_cast<Char>(' '))
            {
                // substr
                cliArguments.Add(text.Subtext(argStart, i - argStart));
                argStart = i + 1;
            }
        }

        if (ch == 0)
            break;
    }

    if (length == argStart)
        return;

    cliArguments.Add(text.Subtext(argStart, length - argStart));
}
