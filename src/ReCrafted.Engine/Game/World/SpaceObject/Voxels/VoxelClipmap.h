// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef VOXELCLIPMAP_H
#define VOXELCLIPMAP_H

// includes
#include "ReCrafted.h"

struct Vector3;
struct SpaceObjectSettings;

struct CSGMode
{
    enum _enum
    {
        Additive,
        Subtractive
    };
};

struct CSGShape
{
    enum _enum
    {
        Cube,
        Sphere
    };
};

class VoxelClipmap : IDisposable
{
public:
    void init(SpaceObjectSettings& settings);
    void dispose() override;

public:
    uint32_t addCSGShape(CSGMode::_enum mode, CSGShape::_enum shape, const Vector3& position, const float size);
    void removeCSGShape(uint32_t id);

public:
    bool chunkModified(const Vector3& position, int lod);
    void chunkModify(const Vector3& position, int lod, sbyte* voxels);
};

#endif VOXELCLIPMAP_H