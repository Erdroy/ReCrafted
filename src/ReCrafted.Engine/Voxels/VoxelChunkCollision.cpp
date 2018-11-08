// ReCrafted (c) 2016-2018 Always Too Late

#include "VoxelChunkCollision.h"
#include "Physics/PhysicsSystem.h"

void VoxelChunkCollsion::BuildCollision(IPhysicsShapeCooker* cooker, Vector3* vertices, size_t vertexCount, uint32_t* indices, size_t indexCount)
{
    m_cooker = cooker;
    m_triangleMesh = cooker->CookTriangleMesh(vertices, vertexCount, indices, indexCount);
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
        m_triangleMesh = nullptr;
    }

    ASSERT(m_physicsShape);

    m_physicsActor = actor;
    m_physicsActor->AttachShape(m_physicsShape);
}

void VoxelChunkCollsion::DetachCollision()
{
    ASSERT(IS_MAIN_THREAD());

    if(m_physicsActor && m_physicsShape)
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
