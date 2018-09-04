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

    var vertexMaterial = SetupMaterial(m_currentMaterialMap, GetMinimalCorner(corners));
    var originalBlend = vertexMaterial.Blend;

    cvar vertexCount = cellData.GetVertexCount();
    for(var i = 0; i < vertexCount; i ++)
    {
        vertexMaterial.Blend = originalBlend;

        cvar edgeCode = RegularVertexData[caseCode][i];

        cvar v0 = edgeCode & 0x0F;
        cvar v1 = (edgeCode & 0xF0) >> 4;

        cvar indexId = CalculateEdgeVertexId(voxelOffset, edgeCode >> 8);

        DEBUG_ASSERT(indexId >= 0);

        cvar resuseIndex = m_vertexReuse[indexId];
        cvar reuseVertex = resuseIndex > -1;

        cvar cornerPos0 = voxelOffset + CellCorner[v0];
        cvar cornerPos1 = voxelOffset + CellCorner[v1];

        cvar intersectionPosition = GetIntersection(cornerPos0, cornerPos1, corners[v0].value, corners[v1].value) * voxelScale;
        cvar vertexPosition = position + intersectionPosition;

        var createVertex = true;
        var duplicatedVertex = false;

        if (reuseVertex)
        {
            cvar reuseMaterial = m_materials[resuseIndex];
            if (reuseMaterial == vertexMaterial) 
            {
                indices[i] = m_vertexReuse[indexId];
                DEBUG_ASSERT(indices[i] < m_vertices.Count());
                createVertex = false;
            }
            else
            {
                duplicatedVertex = true;
            }
        }

        if (duplicatedVertex)
        {
            m_materials[m_vertexReuse[indexId]].Blend = 0u;
        }
        
        if(createVertex)
        {
            cvar index = m_vertices.Count();
            m_vertices.Add(vertexPosition); // TODO: Optimize vertex count -> caused by normalCorrection
            m_normals.Add(Vector3::Zero());

            if (EnableColorChannelGeneration)
                m_colors.Add(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

            if (EnableMaterialChannelGeneration)
                m_materials.Add(vertexMaterial);

            indices[i] = index;
            m_vertexReuse[indexId] = index;
        }
    }

    cvar indexCount = cellData.GetTriangleCount() * 3;
    for (var i = 0; i < indexCount; i += 3)
    {
        cvar vertexIndex0 = indices[cellData.vertexIndex[i + 0]];
        cvar vertexIndex1 = indices[cellData.vertexIndex[i + 1]];
        cvar vertexIndex2 = indices[cellData.vertexIndex[i + 2]];

        DEBUG_ASSERT(vertexIndex0 < m_vertices.Count());
        DEBUG_ASSERT(vertexIndex1 < m_vertices.Count());
        DEBUG_ASSERT(vertexIndex2 < m_vertices.Count());

        cvar vertex0 = m_vertices[vertexIndex0];
        cvar vertex1 = m_vertices[vertexIndex1];
        cvar vertex2 = m_vertices[vertexIndex2];

        if (Vector3::Dot(vertex0, vertex1) < FLT_EPSILON || 
            Vector3::Dot(vertex1, vertex2) < FLT_EPSILON || 
            Vector3::Dot(vertex2, vertex0) < FLT_EPSILON)
            continue; // zero-space triangle

        cvar normal = Vector3::Cross(vertex1 - vertex0, vertex2 - vertex0);

        m_normals[vertexIndex0] += normal;
        m_normals[vertexIndex1] += normal;
        m_normals[vertexIndex2] += normal;
        
        if (!normalCorrection)
        {
            m_indices.Add(vertexIndex0);
            m_indices.Add(vertexIndex1);
            m_indices.Add(vertexIndex2);
        }
    }

}

void TransvoxelMesher::Generate(IVoxelMaterialMap* materialMap, const Vector3& position, int lod, uint8_t borders, Voxel* data)
{
    m_currentMaterialMap = materialMap;

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

    // Normalize the normal to be a normal normal.
    for (rvar normal : m_normals)
        normal.Normalize();
}

void TransvoxelMesher::Apply(const RefPtr<Mesh>& mesh)
{
    // ReSharper disable CppUnreachableCode
    mesh->SetVertices(m_vertices.Data(), m_vertices.Count());
    mesh->SetNormals(m_normals.Data());

    if(EnableColorChannelGeneration)
    {
        mesh->SetColors(m_colors.Data());
    }

    if (EnableMaterialChannelGeneration)
    {
        mesh->SetCustom(m_materials.Data(), sizeof(uint64_t));
    }

    mesh->SetIndices(m_indices.Data(), m_indices.Count());

    mesh->ApplyChanges();

    Clear();
    // ReSharper restore CppUnreachableCode
}

void TransvoxelMesher::Clear()
{
    // cleanup all arrays
    m_vertices.Clear();
    m_normals.Clear();
    m_colors.Clear();
    m_materials.Clear();
    m_indices.Clear();


    if (EnableVertexReuse) 
    {
        // Clean vertex reuse array
        for (var i = 0u; i < m_vertexReuse.Count(); i++)
            m_vertexReuse[i] = -1;
    }
}
