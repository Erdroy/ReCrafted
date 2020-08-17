// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

enum class AssetType : byte
{
    Unknown,

    Texture2D,
    Model,
    Shader,

    VoxelMaterial,

    Material,

    VoxelObject,
    VoxelObjectData,

    AudioSound,

    Count
};
