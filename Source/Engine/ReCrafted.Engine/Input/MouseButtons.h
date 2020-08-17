// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

enum class ButtonState : sbyte
{
    Unknown = -1,

    Down,
    Up,

    Count
};

enum class Button : byte
{
    Unknown,

    Left,
    Middle,
    Right,
    X1,
    X2,

    Count
};
