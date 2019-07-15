// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

// includes
#include <ReCrafted.h>

#include "PhysX.h"

class ShapeCooker
{
public:
    struct Settings
    {
        bool enableWelding = true;
        float weldingThreshold = 0.01f;
    };

private:
    PxCooking* m_cooking = nullptr;

public:
    ShapeCooker(PxFoundation* foundation, PxTolerancesScale& tolerances, const Settings& settings = {});
    ~ShapeCooker();

public:
    PxTriangleMesh* CookTriangleMesh(Vector3* vertices, size_t vertexCount, const uint32_t* indices, size_t indexCount) const;
};
