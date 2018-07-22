// ReCrafted (c) 2016-2018 Always Too Late

#include "TransvoxelMesher.h"
#include "TransvoxelTables.hpp"

#include "Graphics/Mesh.h"

#include "../CommonTables.hpp"

inline int CalculateVertexId(const Int3& pos)
{
    return  (pos.x + 2) * 3 * (VoxelChunkData::ChunkDataSize * 3) * (VoxelChunkData::ChunkDataSize * 3) +
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

void TransvoxelMesher::PolygonizeRegularCell(const Vector3& position, sbyte* data, const float voxelScale, const Int3& voxelOffset, const bool normalCorrection)
{
    byte caseCode = 0;
    for (var i = 0; i < 8; i++) // TODO: unroll
    {
        if (GetVoxel(data, voxelOffset + CellCorner[i]) > 0)
            caseCode |= 1 << i;
    }

    if (caseCode == 0 || caseCode == 255)
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
        DEBUG_ASSERT(indexId < m_vertexReuse.Size());

        if (m_vertexReuse[indexId] == -1)
        {
            cvar cornerPos0 = voxelOffset + CellCorner[v0];
            cvar cornerPos1 = voxelOffset + CellCorner[v1];

            var cornerPos0Float = Vector3(float(cornerPos0.x), float(cornerPos0.y), float(cornerPos0.z));
            var cornerPos1Float = Vector3(float(cornerPos1.x), float(cornerPos1.y), float(cornerPos1.z));

            cvar cornerSample0 = GetVoxel(data, cornerPos0);
            cvar cornerSample1 = GetVoxel(data, cornerPos1);

            cvar intersectionPosition = GetIntersection(cornerPos0Float, cornerPos1Float, cornerSample0, cornerSample1) * voxelScale;
            cvar vertexPosition = position + intersectionPosition;

            m_vertices.Add(vertexPosition); // TODO: Optimize vertex count -> caused by normalCorrection
            m_normals.Add(Vector3::Zero());
            m_colors.Add(Vector4(0.35f, 0.35f, 0.35f, 1.0f));

            cvar index = m_vertices.Count() - 1;

            indices[i] = index;
            m_vertexReuse[indexId] = index;
        }
        else
        {
            indices[i] = m_vertexReuse[indexId];
            DEBUG_ASSERT(indices[i] < m_vertices.Count());
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

void TransvoxelMesher::Generate(const Vector3& position, int lod, uint8_t borders, sbyte* data)
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

                PolygonizeRegularCell(position, data, voxelScale, voxelOffset, normalCorrection);

                // TODO: Transition cell implementation
            }
        }
    }

    for (rvar normal : m_normals)
    {

        // Normalize the normal to be a normal normal.
        normal.Normalize();
    }

    // TODO: Calculate UV's
}

void TransvoxelMesher::Apply(const RefPtr<Mesh>& mesh)
{
    // ReSharper disable CppUnreachableCode
    mesh->SetVertices(m_vertices.Data(), m_vertices.Count());
    mesh->SetNormals(m_normals.Data());

    if(EnableUVChannelGeneration)
    {
        mesh->SetUVs(m_uvs.Data());
    }

    if(EnableColorChannelGeneration)
    {
        mesh->SetColors(m_colors.Data());
    }

    if (EnableMaterialChannelGeneration)
    {
        //mesh->SetColors(m_materials.Data(), 1);
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
    m_uvs.Clear();
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
