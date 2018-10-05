// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ASSETTYPE_H
#define ASSETTYPE_H

enum class AssetType : byte
{
    Unknown,

    Texture2D,
    Model,
    Shader,

    VoxelMaterial,

    Count
};

#endif // ASSETTYPE_H
