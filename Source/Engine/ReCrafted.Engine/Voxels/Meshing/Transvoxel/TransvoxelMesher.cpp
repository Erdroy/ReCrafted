// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "TransvoxelMesher.h"
#include "Rendering/Mesh.h"

void TransvoxelMesher::Generate(const Vector3& position, int lod, uint8_t borders, Voxel* data)
{

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

                // TODO: Transition cell implementation
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

void TransvoxelMesher::Apply(const RefPtr<VoxelChunkMesh>& chunkMesh, RefPtr<VoxelChunkCollision>& chunkCollision)
{
    // Create section mesh
    const auto mesh = Mesh::CreateMesh();

    auto chunkSection = VoxelChunkMesh::MeshSection();
    chunkSection.mesh = mesh;

    auto meshSection = GetSection();
    for(auto material : meshSection.materialSet)
        chunkSection.materialSet.Add(material);

    if (meshSection.indices.Count() == 0)
        return;

    mesh->SetVertices(meshSection.vertices.Data(), meshSection.vertices.Count());
    mesh->SetNormals(meshSection.normals.Data());
    mesh->AddCustomData(meshSection.materialsA.Data(), sizeof(Vector4));
    mesh->AddCustomData(meshSection.materialsB.Data(), sizeof(Vector4));
    mesh->AddCustomData(meshSection.materialsC.Data(), sizeof(Vector4));
    mesh->AddCustomData(meshSection.materialsD.Data(), sizeof(Vector4));
    mesh->SetIndices(meshSection.indices.Data(), meshSection.indices.Count());
    mesh->ApplyChanges();

    // Add mesh section
    chunkMesh->AddSection(chunkSection);

    if(chunkCollision && m_collisionIndices.Count() >= 3)
    {
        // Apply collision
        chunkCollision->BuildCollision(m_shapeCooker, m_currentVoxelScale,
            m_collisionVertices.Data(), m_collisionVertices.Count(), 
            m_collisionIndices.Data(), m_collisionIndices.Count());
    }
    
    Clear();
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
