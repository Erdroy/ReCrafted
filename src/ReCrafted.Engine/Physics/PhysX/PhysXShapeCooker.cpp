// ReCrafted (c) 2016-2018 Always Too Late

#include "PhysXShapeCooker.h"

extern PxPhysics* GPxPhysX;
extern PxFoundation* GPxFoundation;
extern PxTolerancesScale GPxTolerances;

void PhysXShapeCooker::Initialize(const Settings& settings)
{
    // Initialize px cooking, this will be needed for ship colliders cooking etc.
    PxCookingParams cookingParams(GPxTolerances);
    
    if(settings.enableWelding)
    {
        cookingParams.meshWeldTolerance = settings.weldingThreshold; // 10 mm tolerance
        cookingParams.meshPreprocessParams = PxMeshPreprocessingFlags(PxMeshPreprocessingFlag::eWELD_VERTICES);
    }

    cookingParams.meshCookingHint = PxMeshCookingHint::eCOOKING_PERFORMANCE;
    cookingParams.targetPlatform = PxPlatform::ePC;
    cookingParams.midphaseDesc = PxMeshMidPhase::eBVH34;

    m_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *GPxFoundation, cookingParams);

    _ASSERT_(m_cooking, "Failed to create PhysX cooking!");
}

void PhysXShapeCooker::Shutdown()
{
    ASSERT(m_cooking);

    m_cooking->release();
    m_cooking = nullptr;
}

void* PhysXShapeCooker::CookConvexHullMesh(Vector3* vertices, size_t vertexCount, uint32_t* indices,
    size_t indexCount) const
{
    // TODO: Convex hull cooker implementation
    return nullptr;
}

void* PhysXShapeCooker::CookTriangleMesh(Vector3* vertices, size_t vertexCount, uint32_t* indices,
    size_t indexCount) const
{
    // Create triangle mesh description
    PxTriangleMeshDesc meshDescription;
    meshDescription.setToDefault();

    meshDescription.points.data = vertices;
    meshDescription.points.count = PxU32(vertexCount);
    meshDescription.points.stride = sizeof(*vertices);

    meshDescription.triangles.data = indices;
    meshDescription.triangles.count = PxU32(indexCount / 3);
    meshDescription.triangles.stride = 3 * sizeof(*indices);

    ASSERT(meshDescription.isValid());

    // Cook and return (note: in some cases the resulting triangle mesh can be null!)
    return m_cooking->createTriangleMesh(meshDescription, GPxPhysX->getPhysicsInsertionCallback());
}

void PhysXShapeCooker::ReleaseConvexMeshMesh(void* triangleMesh) const
{
    // TODO: Release convex hull mesh
}

void PhysXShapeCooker::ReleaseTriangleMesh(void* triangleMesh) const
{
    cvar pxMesh = static_cast<PxTriangleMesh*>(triangleMesh);
    pxMesh->release();
}
