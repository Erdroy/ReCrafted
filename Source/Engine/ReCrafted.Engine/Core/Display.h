// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

/// <summary>
///     The display class.
/// </summary>
API_CLASS(public, static, partial, noinherit)
class Display final
{
    API_CLASS_BODY()
    friend class Application;

private:
    static uint m_width;
    static uint m_height;

private:
    static void ForceUpdate();

public:
    /// <summary>
    ///     Gets the aspect ratio of the screen.
    /// </summary>
    API_PROPERTY(noprefix)
    static float GetAspectRatio();

    /// <summary>
    ///     Gets the screen width.
    /// </summary>
    API_PROPERTY(noprefix)
    static uint GetWidth();

    /// <summary>
    ///     Gets the screen height.
    /// </summary>
    API_PROPERTY(noprefix)
    static uint GetHeight();
};
