// ReCrafted (c) 2016-2019 Always Too Late

#include "Collision.h"
#include "PhysicsManager.h"

void Collision::OnDestroy()
{
    PhysicsManager::Engine()->ReleaseShape(m_shape);
}

Collision* Collision::CreateInstance()
{
    MAIN_THREAD_ONLY();

    return Object::CreateInstance<Collision>("ReCrafted.API.Physics", "Collision");
}

Collision* Collision::CreateBoxCollision(const Vector3& extents)
{
    MAIN_THREAD_ONLY();

    var collision = CreateInstance();
    collision->m_type = Box;
    collision->m_shape = PhysicsManager::Engine()->CreateBoxShape(extents);
    return collision;
}

Collision* Collision::CreateSphereCollision(const float radius)
{
    MAIN_THREAD_ONLY();

    var collision = CreateInstance();
    collision->m_type = Sphere;
    collision->m_shape = PhysicsManager::Engine()->CreateSphereShape(radius);
    return collision;
}

Collision* Collision::CreateCapsuleCollision(const float radius, const float halfHeight)
{
    MAIN_THREAD_ONLY();

    var collision = CreateInstance();
    collision->m_type = Capsule;
    collision->m_shape = PhysicsManager::Engine()->CreateCapsuleShape(radius, halfHeight);
    return collision;
}

Collision* Collision::CreateTriangleMeshCollision(Vector3* points, const size_t numPoints, uint32_t* indices,
                                                  const size_t numIndices, IPhysicsShapeCooker* shapeCooker)
{
    MAIN_THREAD_ONLY();

    if (!shapeCooker)
        shapeCooker = PhysicsManager::Engine()->GetDefaultCooker();

    // Cook shape
    cvar shape = shapeCooker->CookTriangleMesh(points, numPoints, indices, numIndices);

    if (shape == nullptr)
        return nullptr;

    var collision = CreateInstance();
    collision->m_type = TriangleMesh;
    collision->m_shape = PhysicsManager::Engine()->CreateTriangleMeshShape(shape);

    // Release shape
    shapeCooker->ReleaseTriangleMesh(shape);

    return collision;
}

Collision* Collision::CreateTriangleMeshCollision(void* triangleMeshShape, IPhysicsShapeCooker* shapeCooker)
{
    MAIN_THREAD_ONLY();

    if (!shapeCooker)
        shapeCooker = PhysicsManager::Engine()->GetDefaultCooker();

    var collision = CreateInstance();
    collision->m_type = TriangleMesh;
    collision->m_shape = PhysicsManager::Engine()->CreateTriangleMeshShape(triangleMeshShape);

    // Release shape
    shapeCooker->ReleaseTriangleMesh(triangleMeshShape);

    return collision;
}

Collision* Collision::CreateConvexHullMeshCollision(Vector3* points, const size_t numPoints, uint32_t* indices,
                                                    const size_t numIndices, IPhysicsShapeCooker* shapeCooker)
{
    MAIN_THREAD_ONLY();

    if (!shapeCooker)
        shapeCooker = PhysicsManager::Engine()->GetDefaultCooker();

    // Cook shape
    cvar shape = shapeCooker->CookConvexHullMesh(points, numPoints, indices, numIndices);

    if (shape == nullptr)
        return nullptr;

    var collision = CreateInstance();
    collision->m_type = ConvexHullMesh;
    collision->m_shape = PhysicsManager::Engine()->CreateConvexHullMeshShape(shape);

    // Release shape
    shapeCooker->ReleaseConvexMeshMesh(shape);

    return collision;
}

Collision* Collision::CreateConvexHullMeshCollision(void* convexHullMeshShape, IPhysicsShapeCooker* shapeCooker)
{
    MAIN_THREAD_ONLY();

    if (!shapeCooker)
        shapeCooker = PhysicsManager::Engine()->GetDefaultCooker();

    var collision = CreateInstance();
    collision->m_type = ConvexHullMesh;
    collision->m_shape = PhysicsManager::Engine()->CreateConvexHullMeshShape(convexHullMeshShape);

    // Release shape
    shapeCooker->ReleaseConvexMeshMesh(convexHullMeshShape);


    return collision;
}
