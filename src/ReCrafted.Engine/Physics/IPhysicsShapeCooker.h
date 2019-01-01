// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef IPHYSICSSHAPECOOKER_H
#define IPHYSICSSHAPECOOKER_H

// includes
#include "ReCrafted.h"

class IPhysicsShape;

class IPhysicsShapeCooker
{
public:
    struct Settings
    {
        bool enableWelding = true;
        float weldingThreshold = 0.01f;
    };

public:
    virtual ~IPhysicsShapeCooker() = default;

public:
    virtual void Initialize(const Settings& settings) = 0;
    virtual void Shutdown() = 0;

public:
    virtual void* CookConvexHullMesh(Vector3* vertices, size_t vertexCount, uint32_t* indices, size_t indexCount) const = 0;
    virtual void* CookTriangleMesh(Vector3* vertices, size_t vertexCount, uint32_t* indices, size_t indexCount) const = 0;

    virtual void ReleaseConvexMeshMesh(void* triangleMesh) const = 0;
    virtual void ReleaseTriangleMesh(void* triangleMesh) const = 0;
};

#endif // IPHYSICSSHAPECOOKER_H
