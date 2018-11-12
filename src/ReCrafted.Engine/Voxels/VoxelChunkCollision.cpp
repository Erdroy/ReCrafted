// ReCrafted (c) 2016-2018 Always Too Late

#include "VoxelChunkCollision.h"
#include "Physics/PhysicsManager.h"
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

    if (!m_triangleMesh)
        Logger::LogWarning("Couldn't cook collision information for {0} vertices and {1} indices!", vertexCount, indexCount);
}

void VoxelChunkCollsion::AttachCollision(RigidBodyActor* actor)
{
    ASSERT(IS_MAIN_THREAD());
    ASSERT(actor);

    if(m_triangleMesh)
    {
        ASSERT(m_collision == nullptr);

        m_collision = Collision::CreateTriangleMeshCollision(m_triangleMesh, m_cooker);
        m_triangleMesh = nullptr;
    }

    ASSERT(m_collision);

    m_physicsActor = actor;
    m_physicsActor->AttachCollision(m_collision);
}

void VoxelChunkCollsion::DetachCollision()
{
    ASSERT(IS_MAIN_THREAD());
    ASSERT(m_physicsActor);
    ASSERT(m_collision);

    m_physicsActor->DetachCollision();
    m_physicsActor = nullptr;
}

void VoxelChunkCollsion::Dispose()
{
    ASSERT(IS_MAIN_THREAD());

    // Detach collsion if attached
    if (IsAttached())
    {
        DetachCollision();
    }

    if(m_triangleMesh)
    {
        ASSERT(m_cooker);

        m_cooker->ReleaseTriangleMesh(m_triangleMesh);
        m_triangleMesh = nullptr;
    }

    if(m_collision)
    {
        Object::Destroy(m_collision);
    }
}
