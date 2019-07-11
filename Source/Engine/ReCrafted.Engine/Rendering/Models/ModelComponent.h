// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

struct ModelComponent
{
public:
    bool Active = false;
    Mesh* Mesh = nullptr;
    Material* Material = nullptr;
    Transform* Transform = nullptr;
    BoundingBox Bounds = {};

    // TODO: Transform reference?
};
