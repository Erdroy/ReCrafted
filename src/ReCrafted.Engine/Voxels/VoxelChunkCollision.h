// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef VOXELCHUNKCOLLISION_H
#define VOXELCHUNKCOLLISION_H

// includes
#include "ReCrafted.h"
#include "Core/IDisposable.h"
#include "Physics/IPhysicsShape.h"
#include "Physics/IPhysicsActor.h"
#include "Physics/IPhysicsShapeCooker.h"

class VoxelChunkCollsion : public IDisposable
{
private:
    IPhysicsShape* m_physicsShape = nullptr;
    IPhysicsActor* m_physicsActor = nullptr;

    IPhysicsShapeCooker* m_cooker = nullptr;

    void* m_triangleMesh = nullptr;
    void* m_convexHullMesh = nullptr;

public:
    void BuildCollision(IPhysicsShapeCooker* cooker, float voxelScale, Vector3* vertices, size_t vertexCount, uint32_t* indices, size_t indexCount);
    void AttachCollision(IPhysicsActor* actor);
    void DetachCollision();

public:
    bool IsAttached() const
    {
        return m_physicsActor != nullptr;
    }

    bool IsValid() const
    {
        return m_triangleMesh || m_convexHullMesh || m_physicsShape;
    }

public:
    /**
     * \brief Disposes chunk collision.
     */
    void Dispose() override;
};

#endif // VOXELCHUNKCOLLISION_H
