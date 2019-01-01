// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef MOUSEBUTTONS_H
#define MOUSEBUTTONS_H

#include "ReCrafted.h"

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

#endif // MOUSEBUTTONS_H
