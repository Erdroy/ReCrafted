// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

/// <summary>
///     Defines all possible types of fields that can be passed to Material class.
/// </summary>
enum class MaterialFieldType : byte
{
    Unknown,

    Float1,
    Float2,
    Float4,
    Boolean,
    Color32,
    Custom,

    Count
};