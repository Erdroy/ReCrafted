// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "TransvoxelMesher.h"
#include "Rendering/Mesh.h"
#include "Voxels/VoxelChunkMesh.h"
#include "Physics/Colliders/MeshCollider.h"
#include "Profiler/Profiler.h"

void TransvoxelMesher::Generate(const Vector3& position, const int lod, uint8_t borders, Voxel* data)
{
    CPU_PROFILE_FUNCTION(0);

    // ReSharper disable once CppUnreachableCode
    const auto voxelScale = static_cast<float>(lod);

    m_currentVoxelScale = voxelScale;

    for(auto x = VoxelChunkData::ChunkDataStart; x < VoxelChunkData::ChunkDataLength - 1; x ++)
    {
        for (auto y = VoxelChunkData::ChunkDataStart; y < VoxelChunkData::ChunkDataLength - 1; y++)
        {
            for (auto z = VoxelChunkData::ChunkDataStart; z < VoxelChunkData::ChunkDataLength - 1; z++)
            {
                const auto voxelOffset = Vector3i(x, y, z);

                const auto normalCorrection = x < 0 || y < 0 || z < 0 || x >= 16 || y >= 16 || z >= 16;

                PolygonizeRegularCell(position, data, voxelScale, lod, voxelOffset, normalCorrection);

                // TODO: Transition cell implementation (we need access to neighbor chunk cells (with lower LoD [LoD 0 - highest details, so 'lod + 1' or more]))
            }
        }
    }

    // Apply normals
    for(auto& section : m_meshSections)
    {
        // Normalize the normals to be a normal normals.
        auto j = 0;
        for (auto& normal : section.normals)
        {
            const auto vertexId = section.vertexIds[j];
            const auto vertexInfo = m_vertexInfo[vertexId];

            normal = vertexInfo.vertexNormal;
            normal.Normalize();

            j++;
        }
    }
}

bool TransvoxelMesher::ValidateTriangles(const int maxCount)
{
    if (m_collisionIndices.Empty())
        return false;

    const auto numSectionTriangles = static_cast<int>(m_collisionIndices.Count());
    const auto numVertices = std::min(numSectionTriangles, maxCount * 3);

    for (auto i = 0; i < numVertices; i += 3)
    {
        const auto i0 = m_collisionIndices[i + 0];
        const auto i1 = m_collisionIndices[i + 1];
        const auto i2 = m_collisionIndices[i + 2];

        // If there is at least one index with the same value, we have invalid triangle. Return false.
        if (i0 == i1 || i1 == i2 || i2 == i0)
            return false;

        // Check for position difference
        const auto v0 = m_collisionVertices[i0];
        const auto v1 = m_collisionVertices[i1];
        const auto v2 = m_collisionVertices[i2];

        const auto minDifference = 0.00001f;

        // If there is at least one vertex with the same value, we have invalid triangle. Return false.
        if (v0 == v1 || v1 == v2 || v2 == v0)
            return false;
    }


    // Mesh is ok. Return true.
    return true;
}

bool TransvoxelMesher::Apply(const RefPtr<VoxelChunkMesh>& chunkMesh, MeshCollider* chunkCollision)
{
    CPU_PROFILE_FUNCTION(0);

    // Create mesh
    const auto mesh = Mesh::CreateMesh();

    // Create chunk section
    auto chunkSection = VoxelChunkMesh::MeshSection();
    chunkSection.mesh = mesh;

    auto meshSection = GetSection();
    for(auto material : meshSection.materialSet)
        chunkSection.materialSet.Add(material);

    if (meshSection.indices.Count() == 0)
        return false;

    mesh->SetVertices(Array<Vector3>(meshSection.vertices.Data(), meshSection.vertices.Size()));
    mesh->SetNormals(Array<Vector3>(meshSection.normals.Data(), meshSection.vertices.Size()));
    mesh->AddCustomData(meshSection.materialsA.Data(), sizeof(Vector4));
    mesh->AddCustomData(meshSection.materialsB.Data(), sizeof(Vector4));
    mesh->AddCustomData(meshSection.materialsC.Data(), sizeof(Vector4));
    mesh->AddCustomData(meshSection.materialsD.Data(), sizeof(Vector4));
    mesh->SetIndices(Array<uint>(meshSection.indices.Data(), meshSection.indices.Count()));
    mesh->ApplyChanges(true, false);

    // Add mesh section
    chunkMesh->AddSection(chunkSection);

    if(chunkCollision && HasTriangles())
    {
        // Bake the mesh
        chunkCollision->SetMesh(
            Array<Vector3>(m_collisionVertices.Data(), m_collisionVertices.Count()), 
            Array<uint>(m_collisionIndices.Data(), m_collisionIndices.Count()), 
            m_shapeCooker
        );

        if (!chunkCollision->IsValid())
            return false;
    }
    
    Clear();
    return true;
}

void TransvoxelMesher::Clear()
{
    m_triangleCount = 0u;

    // Clear only used sections
    for (auto i = 0; i <= m_currentSection; i ++)
        m_meshSections[i].Clear();

    // Reset vertex info
    //m_vertexInfo.Clear();
    m_vertexInfoMap.reset();

    m_collisionVertices.Clear();
    m_collisionIndices.Clear();

    m_currentSection = 0;
}
