// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Environment.h"

List<String> Environment::m_arguments;

void Environment::SetCommandLine(const String& string)
{
    const auto length = string.Length();
    auto isString = false;
    auto argStart = 0;

    for (auto i = 0; i < length; i++)
    {
        const auto ch = string[i];

        if (ch == static_cast<Char>('\"'))
        {
            if (isString)
            {
                // substr
                m_arguments.Add(string.Substring(argStart, i - argStart));
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
                m_arguments.Add(string.Substring(argStart, i - argStart));
                argStart = i + 1;
            }
        }

        if (ch == 0)
            break;
    }

    if (length == argStart)
        return;

    m_arguments.Add(string.Substring(argStart, length - argStart));
}

const List<String>& Environment::GetCommandLineArguments()
{
    return m_arguments;
}

bool Environment::IsDebugBuild()
{
#ifdef DEBUG
    return true;
#else
    return false;
#endif
}

bool Environment::IsReleaseBuild()
{
    // TODO: Update when we add additional Development build.
    return !IsDebugBuild();
}
