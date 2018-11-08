// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PHYSXSHAPECOOKER_H
#define PHYSXSHAPECOOKER_H

// includes
#include "ReCrafted.h"
#include "Physics/IPhysicsShapeCooker.h"

#include "PhysX.h"

class PhysXShapeCooker : public IPhysicsShapeCooker
{
private:
    PxCooking* m_cooking = nullptr;

public:
    void Initialize(const Settings& settings) override;
    void Shutdown() override;

public:
    void* CookConvexHullMesh(Vector3* vertices, size_t vertexCount, uint32_t* indices, size_t indexCount) const override;
    void* CookTriangleMesh(Vector3* vertices, size_t vertexCount, uint32_t* indices, size_t indexCount) const override;
};

#endif // PHYSXSHAPECOOKER_H
