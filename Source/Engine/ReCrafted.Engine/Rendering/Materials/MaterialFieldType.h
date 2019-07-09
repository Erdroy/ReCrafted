// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

/// <summary>
///     Defines all possible types of fields that can be passed to Material class.
/// </summary>
enum class MaterialFieldType : byte
{
    Unknown = 0,

    Float,
    Float1 = Float,
    Float2,
    Float3,
    Float4,
    Color32 = Float4,

    Count
};