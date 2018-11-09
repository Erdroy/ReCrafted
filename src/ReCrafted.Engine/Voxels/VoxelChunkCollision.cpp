// ReCrafted (c) 2016-2018 Always Too Late

#include "VoxelChunkCollision.h"
#include "Physics/PhysicsSystem.h"
#include "Core/Logger.h"

void VoxelChunkCollsion::BuildCollision(IPhysicsShapeCooker* cooker, float voxelScale, Vector3* vertices, size_t vertexCount, uint32_t* indices, size_t indexCount)
{
    m_cooker = cooker;

    cvar convexHullThreshold = 8.0f;

    // Determine which algorithm based on voxelScale, we are going to use.
    //if(voxelScale >= convexHullThreshold)
    //    m_convexHullMesh = cooker->CookConvexHullMesh(vertices, vertexCount); // TODO: Convex mesh bake
    //else
    m_triangleMesh = cooker->CookTriangleMesh(vertices, vertexCount, indices, indexCount);

    if (!m_triangleMesh && !m_convexHullMesh)
        Logger::LogWarning("Couldn't cook collision information for {0} vertices and {1} indices!", vertexCount, indexCount);
}

void VoxelChunkCollsion::AttachCollision(IPhysicsActor* actor)
{
    ASSERT(IS_MAIN_THREAD());
    ASSERT(actor);

    if(m_triangleMesh)
    {
        cvar physics = PhysicsSystem::Physics();
        cvar transform = TransformComponent();

        var shape = PhysicsShapeComponent(PhysicsShapeComponent::TriangleMesh);

        // Set shape pointer
        shape.shapePointer = m_triangleMesh;

        cvar physicsShape = physics->CreateShape(transform, shape);

        ASSERT(physicsShape);

        m_physicsShape = physicsShape;

        // Cleanup and free temporary memory
        m_cooker->ReleaseTriangleMesh(m_triangleMesh);
        m_triangleMesh = nullptr;
    }
    
    if(m_convexHullMesh)
    {
        cvar physics = PhysicsSystem::Physics();
        cvar transform = TransformComponent();

        var shape = PhysicsShapeComponent(PhysicsShapeComponent::ConvexHull);

        // Set shape pointer
        shape.shapePointer = m_convexHullMesh;

        cvar physicsShape = physics->CreateShape(transform, shape);

        ASSERT(physicsShape);

        m_physicsShape = physicsShape;

        // Cleanup and free temporary memory
        m_cooker->ReleaseConvexMeshMesh(m_convexHullMesh);
        m_convexHullMesh = nullptr;
    }

    ASSERT(m_physicsShape);

    m_physicsActor = actor;
    m_physicsActor->AttachShape(m_physicsShape);
}

void VoxelChunkCollsion::DetachCollision()
{
    ASSERT(IS_MAIN_THREAD());
    ASSERT(m_physicsActor);
    ASSERT(m_physicsShape);

    m_physicsActor->DetachShape(m_physicsShape);
    m_physicsActor = nullptr;
}

void VoxelChunkCollsion::Dispose()
{
    ASSERT(IS_MAIN_THREAD());
 
    if(m_triangleMesh)
    {
        ASSERT(m_cooker);

        m_cooker->ReleaseTriangleMesh(m_triangleMesh);
        m_triangleMesh = nullptr;
    }

    if(m_convexHullMesh)
    {
        ASSERT(m_cooker);

        m_cooker->ReleaseConvexMeshMesh(m_convexHullMesh);
        m_convexHullMesh = nullptr;
    }

    if(m_physicsShape)
    {
        // Detach collsion if attached
        if(IsAttached())
        {
            DetachCollision();
        }

        cvar physics = PhysicsSystem::Physics();
        physics->ReleaseShape(m_physicsShape);
        m_physicsShape = nullptr;
    }
}
