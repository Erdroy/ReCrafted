// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "MeshCollider.h"
#include "Physics/PhysicsManager.h"
#include "Rendering/Mesh.h"

MeshCollider::MeshCollider()
{
    m_material = PhysicsMaterial::Default();
}

MeshCollider::~MeshCollider()
{

}

void MeshCollider::SetMesh(Mesh* mesh)
{
    MAIN_THREAD_ONLY();
    ASSERT(mesh);
    SetMesh(mesh, PhysicsManager::GetShapeCooker());
}

void MeshCollider::SetMesh(Mesh* mesh, ShapeCooker* shapeCooker)
{
    ASSERT(mesh);
    ASSERT(shapeCooker);

    // Validate mesh.
    ASSERT(mesh->Vertices().Data() != nullptr);
    ASSERT(mesh->Indices().Data() != nullptr);
    ASSERT(mesh->VertexCount() > 0);
    ASSERT(mesh->IndexCount() > 0);

    // Set mesh
    SetMesh(mesh->Vertices(),  mesh->Indices(), shapeCooker);
}

void MeshCollider::SetMesh(const Array<Vector3>& vertices, const Array<uint32_t>& indices)
{
    MAIN_THREAD_ONLY();
    SetMesh(vertices, indices, PhysicsManager::GetShapeCooker());
}

void MeshCollider::SetMesh(const Array<Vector3>& vertices, const Array<uint32_t>& indices, ShapeCooker* shapeCooker)
{
    ASSERT(vertices.Data() != nullptr);
    ASSERT(indices.Data() != nullptr);
    ASSERT(vertices.Count() > 0);
    ASSERT(indices.Count() > 0);

    // Cook triangle mesh
    auto triangleMesh = shapeCooker->CookTriangleMesh(vertices.Data(), vertices.Count(), indices.Data(), indices.Count());

    if (!triangleMesh)
        return;

    // Create triangle mesh shape
    m_shape = PhysicsManager::GetPhysics()->createShape(PxTriangleMeshGeometry(triangleMesh), *m_material->GetPxMaterial());
    ASSERT(m_shape);

    // Release triangle mesh geometry
    triangleMesh->release();

    PxMaterial* materials[] = { m_material->GetPxMaterial() };
    m_shape->setMaterials(materials, 1u);
}
