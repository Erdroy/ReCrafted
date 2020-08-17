// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "TransvoxelMesher.h"
#include "TransvoxelTables.hpp"

void TransvoxelMesher::PolygonizeRegularCell(const Vector3& position, Voxel* data, const float voxelScale, const int lod, const Vector3i& voxelOffset, const bool normalCorrection)
{
    Voxel corners[8];
    for (auto i = 0; i < 8; i++)
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

    if ((caseCode ^ ((-corners[7].value >> 7) & 0xFF)) == 0)
        return;

    const auto cellClass = RegularCellClass[caseCode];
    const auto cellData = RegularCellData[cellClass];

    uint indices[12] = {};
    uint collisionIndices[12] = {};

    const auto vertexCount = cellData.GetVertexCount();
    for (auto i = 0; i < vertexCount; i++)
    {
        const auto edgeCode = RegularVertexData[caseCode][i];

        const auto v0 = edgeCode & 0x0F;
        const auto v1 = (edgeCode & 0xF0) >> 4;

        const auto vertexId = CalculateEdgeVertexId(voxelOffset, edgeCode >> 8);
        const auto collisionVertexId = m_collisionVertices.Count();
        DEBUG_ASSERT(vertexId >= 0);

        // Skip vertex calculation if already calculated
        if (m_vertexInfoMap.test(vertexId))
        {
            indices[i] = vertexId;
            collisionIndices[i] = m_vertexInfo[vertexId].collisionVertexId;
            continue;
        }

        // Calculate vertex
        const auto voxelA = corners[v0];
        const auto voxelB = corners[v1];

        const auto intersectionPosition = GetIntersection(voxelOffset + CellCorner[v0], voxelOffset + CellCorner[v1], voxelA.value, voxelB.value) * voxelScale;
        const auto vertexPosition = position + intersectionPosition;
        const auto voxelMaterial = GetVoxelMaterial(voxelA, voxelB);

        // Add mesh vertex
        const auto vertexInfo = VertexInfo(vertexId, collisionVertexId, vertexPosition, voxelMaterial);
        m_vertexInfo[vertexId] = vertexInfo;
        indices[i] = vertexId;
        m_vertexInfoMap.set(vertexId);

        // Add collision vertex
        collisionIndices[i] = collisionVertexId;
        m_collisionVertices.Add(vertexPosition);
    }

    const auto indexCount = cellData.GetTriangleCount() * 3;
    for (auto i = 0; i < indexCount; i += 3)
    {
        const auto vertexId0 = indices[cellData.vertexIndex[i + 0]];
        const auto vertexId1 = indices[cellData.vertexIndex[i + 1]];
        const auto vertexId2 = indices[cellData.vertexIndex[i + 2]];

        auto& vertexInfoA = m_vertexInfo[vertexId0];
        auto& vertexInfoB = m_vertexInfo[vertexId1];
        auto& vertexInfoC = m_vertexInfo[vertexId2];

        const auto vertexPosition0 = vertexInfoA.vertexPosition;
        const auto vertexPosition1 = vertexInfoB.vertexPosition;
        const auto vertexPosition2 = vertexInfoC.vertexPosition;

        if (Vector3::Dot(vertexPosition0, vertexPosition1) < Math::ZeroTolerance ||
            Vector3::Dot(vertexPosition1, vertexPosition2) < Math::ZeroTolerance ||
            Vector3::Dot(vertexPosition2, vertexPosition0) < Math::ZeroTolerance)
            continue; // zero-space triangle

        const auto normal = Vector3::Cross(vertexPosition1 - vertexPosition0, vertexPosition2 - vertexPosition0);

        // Calculate normals
        vertexInfoA.vertexNormal += normal;
        vertexInfoB.vertexNormal += normal;
        vertexInfoC.vertexNormal += normal;

        // Add mesh triangle
        AddTriangle(vertexInfoA, vertexInfoB, vertexInfoC, normalCorrection);

        if (!normalCorrection)
        {
            // Add collision triangle
            m_collisionIndices.Add(collisionIndices[cellData.vertexIndex[i + 0]]);
            m_collisionIndices.Add(collisionIndices[cellData.vertexIndex[i + 1]]);
            m_collisionIndices.Add(collisionIndices[cellData.vertexIndex[i + 2]]);
        }

        // Increment triangle count
        m_triangleCount++;
    }
}
