// ReCrafted (c) 2016-2018 Always Too Late

#include "TransvoxelMesher.h"
#include "TransvoxelTables.hpp"

#include "../CommonTables.hpp"

#include "Graphics/Mesh.h"

inline int CalculateVertexId(const Int3& pos)
{
    return (pos.x + 2) * 3 * (VoxelChunkData::ChunkDataSize * 3) * (VoxelChunkData::ChunkDataSize * 3) +
           (pos.y + 2) * 3 * (VoxelChunkData::ChunkDataSize * 3) +
           (pos.z + 2) * 3;
}

inline int CalculateEdgeVertexId(const Int3& pos, int edgeCode)
{
    cvar edge = edgeCode & 0x0F;
    cvar owner = (edgeCode & 0xF0) >> 4;

    cvar diffX = owner % 2;
    cvar diffY = (owner >> 2) % 2;
    cvar diffZ = (owner >> 1) % 2;

    DEBUG_ASSERT((diffX == 0) || (diffX == 1));
    DEBUG_ASSERT((diffY == 0) || (diffY == 1));
    DEBUG_ASSERT((diffZ == 0) || (diffZ == 1));

    return CalculateVertexId(pos - Int3(diffX, diffY, diffZ)) + (edge - 1);
}

void TransvoxelMesher::PolygonizeRegularCell(const Vector3& position, Voxel* data, const float voxelScale, const int lod, const Int3& voxelOffset, const bool normalCorrection)
{
    Voxel corners[8];
    for(var i = 0; i < 8; i ++)
    {
        corners[i] = GetVoxel(data, voxelOffset + CellCorner[i]);
    }

    const byte caseCode =
          (-corners[0].value >> 7 & 0x01)
        | (-corners[1].value >> 6 & 0x02)
        | (-corners[2].value >> 5 & 0x04)
        | (-corners[3].value >> 4 & 0x08)
        | (-corners[4].value >> 3 & 0x10)
        | (-corners[5].value >> 2 & 0x20)
        | (-corners[6].value >> 1 & 0x40)
        | (-corners[7].value & 0x80);

    if((caseCode ^ ((-corners[7].value >> 7) & 0xFF)) == 0)
        return;

    cvar cellClass = RegularCellClass[caseCode];
    cvar cellData = RegularCellData[cellClass];

    uint indices[12] = {};

    cvar vertexCount = cellData.GetVertexCount();
    for(var i = 0; i < vertexCount; i ++)
    {
        cvar edgeCode = RegularVertexData[caseCode][i];

        cvar v0 = edgeCode & 0x0F;
        cvar v1 = (edgeCode & 0xF0) >> 4;

        cvar indexId = CalculateEdgeVertexId(voxelOffset, edgeCode >> 8);

        DEBUG_ASSERT(indexId >= 0);

        cvar cornerPos0 = voxelOffset + CellCorner[v0];
        cvar cornerPos1 = voxelOffset + CellCorner[v1];

        cvar voxelA = corners[v0];
        cvar voxelB = corners[v1];

        cvar intersectionPosition = GetIntersection(cornerPos0, cornerPos1, voxelA.value, voxelB.value) * voxelScale;
        cvar vertexPosition = position + intersectionPosition;
        cvar voxelMaterial = GetVoxelMaterial(voxelA, voxelB);
        cvar vertexMaterial = EncodeMaterial(voxelMaterial);

        // If material set size is 16 and we don't have this voxel material in our section, create new section.
        rvar section = GetSection();
        /*cvar materialPresent = section.HasMaterial(voxelMaterial);
        var newSection = false;

        if(!materialPresent && section.MaterialsFull())
        {
            section = CreateSection();
            newSection = true;
        }*/

        cvar reuseIndex = section.vertexReuse[indexId];
        cvar reuseVertex = reuseIndex > -1;

        if (reuseVertex)
        {
            indices[i] = section.vertexReuse[indexId];
            DEBUG_ASSERT(indices[i] < m_vertices.Count());
            continue;
        }

        cvar index = section.vertices.Count();
        section.vertices.Add(vertexPosition);
        section.normals.Add(Vector3::Zero());
        section.materials.Add(vertexMaterial);

        //if(!materialPresent)
        //    section.materialSet.Add(voxelMaterial);

        indices[i] = index;
        section.vertexReuse[indexId] = index;
    }

    cvar indexCount = cellData.GetTriangleCount() * 3;
    for (var i = 0; i < indexCount; i += 3)
    {
        rvar section = GetSection();

        cvar vertexIndex0 = indices[cellData.vertexIndex[i + 0]];
        cvar vertexIndex1 = indices[cellData.vertexIndex[i + 1]];
        cvar vertexIndex2 = indices[cellData.vertexIndex[i + 2]];

        DEBUG_ASSERT(vertexIndex0 < m_vertices.Count());
        DEBUG_ASSERT(vertexIndex1 < m_vertices.Count());
        DEBUG_ASSERT(vertexIndex2 < m_vertices.Count());

        cvar vertex0 = section.vertices[vertexIndex0];
        cvar vertex1 = section.vertices[vertexIndex1];
        cvar vertex2 = section.vertices[vertexIndex2];

        if (Vector3::Dot(vertex0, vertex1) < Math::Epsilon || 
            Vector3::Dot(vertex1, vertex2) < Math::Epsilon ||
            Vector3::Dot(vertex2, vertex0) < Math::Epsilon)
            continue; // zero-space triangle

        cvar normal = Vector3::Cross(vertex1 - vertex0, vertex2 - vertex0);

        section.normals[vertexIndex0] += normal;
        section.normals[vertexIndex1] += normal;
        section.normals[vertexIndex2] += normal;

        section.normals[vertexIndex0] *= 0.5f;
        section.normals[vertexIndex1] *= 0.5f;
        section.normals[vertexIndex2] *= 0.5f;
        
        if (!normalCorrection)
        {
            section.indices.Add(vertexIndex0);
            section.indices.Add(vertexIndex1);
            section.indices.Add(vertexIndex2);
        }

        // Add to triangle count
        m_triangleCount += 3;
    }
}

void TransvoxelMesher::Generate(const Vector3& position, int lod, uint8_t borders, Voxel* data)
{
    // ReSharper disable once CppUnreachableCode
    cvar voxelScale = static_cast<float>(lod);

    for(var x = VoxelChunkData::ChunkDataStart; x < VoxelChunkData::ChunkDataLength - 1; x ++)
    {
        for (var y = VoxelChunkData::ChunkDataStart; y < VoxelChunkData::ChunkDataLength - 1; y++)
        {
            for (var z = VoxelChunkData::ChunkDataStart; z < VoxelChunkData::ChunkDataLength - 1; z++)
            {
                cvar voxelOffset = Int3(x, y, z);

                cvar normalCorrection = x < 0 || y < 0 || z < 0 || x >= 16 || y >= 16 || z >= 16;

                PolygonizeRegularCell(position, data, voxelScale, lod, voxelOffset, normalCorrection);

                // TODO: Transition cell implementation
            }
        }
    }

    for(var i = 0; i <= m_currentSection; i ++)
    {
        rvar section = m_meshSections[i];

        // Normalize the normals to be a normal normals.
        for (rvar normal : section.normals)
            normal.Normalize();
    }
}

void TransvoxelMesher::Apply(const RefPtr<VoxelChunkMesh>& chunkMesh)
{
    // Create section mesh
    cvar mesh = Mesh::CreateMesh();

    var chunkSection = VoxelChunkMesh::MeshSection();
    chunkSection.mesh = mesh;

    var meshSection = GetSection();
    for(var material : meshSection.materialSet)
        chunkSection.materialSet.Add(material);

    if (meshSection.indices.Count() == 0)
        return;

    mesh->SetVertices(meshSection.vertices.Data(), meshSection.vertices.Count());
    mesh->SetNormals(meshSection.normals.Data());
    mesh->AddCustomData(meshSection.materials.Data(), sizeof(Vector4));
    mesh->SetIndices(meshSection.indices.Data(), meshSection.indices.Count());
    mesh->ApplyChanges();

    // Add mesh section
    chunkMesh->AddSection(chunkSection);

    Clear();
}

void TransvoxelMesher::Clear()
{
    m_triangleCount = 0u;

    // Clear only used sections
    for (var i = 0; i <= m_currentSection; i ++)
        m_meshSections[i].Clear();

    m_currentSection = 0;
}
