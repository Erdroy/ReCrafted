// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "ShapeCooker.h"
#include "PhysicsManager.h"

ShapeCooker::ShapeCooker(PxFoundation* foundation, PxTolerancesScale& tolerances, const Settings& settings)
{
    // Initialize px cooking, this will be needed for ship colliders cooking etc.
    PxCookingParams cookingParams(tolerances);

    if (settings.enableWelding)
    {
        //cookingParams.meshWeldTolerance = settings.weldingThreshold; // 1 cm tolerance
        //cookingParams.meshPreprocessParams = PxMeshPreprocessingFlags(PxMeshPreprocessingFlag::eWELD_VERTICES);
        
    }

    cookingParams.midphaseDesc = PxMeshMidPhase::eBVH34;

    // Create cooking instance
    m_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, cookingParams);

    _ASSERT_(m_cooking, "Failed to create PhysX cooking!");
}

ShapeCooker::~ShapeCooker()
{
    ASSERT(m_cooking);

    m_cooking->release();
    m_cooking = nullptr;
}

PxTriangleMesh* ShapeCooker::CookTriangleMesh(Vector3* vertices, const size_t vertexCount, const uint32_t* indices, const size_t indexCount) const
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

    // Return resulting triangle mesh
    return m_cooking->createTriangleMesh(meshDescription, PhysicsManager::GetPhysics()->getPhysicsInsertionCallback());
}
