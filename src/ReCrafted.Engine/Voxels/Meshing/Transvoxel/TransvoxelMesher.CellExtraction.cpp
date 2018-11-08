// ReCrafted (c) 2016-2018 Always Too Late

#include "TransvoxelMesher.h"
#include "TransvoxelTables.hpp"

void TransvoxelMesher::PolygonizeRegularCell(const Vector3& position, Voxel* data, const float voxelScale, const int lod, const Int3& voxelOffset, const bool normalCorrection)
{
    Voxel corners[8];
    for (var i = 0; i < 8; i++)
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

    cvar cellClass = RegularCellClass[caseCode];
    cvar cellData = RegularCellData[cellClass];

    uint indices[12] = {};
    uint collisionIndices[12] = {};

    cvar vertexCount = cellData.GetVertexCount();
    for (var i = 0; i < vertexCount; i++)
    {
        cvar edgeCode = RegularVertexData[caseCode][i];

        cvar v0 = edgeCode & 0x0F;
        cvar v1 = (edgeCode & 0xF0) >> 4;

        cvar vertexId = CalculateEdgeVertexId(voxelOffset, edgeCode >> 8);
        DEBUG_ASSERT(vertexId >= 0);

        // Skip vertex calculation if already calculated
        if (m_vertexInfoMap.test(vertexId))
        {
            indices[i] = vertexId;
            continue;
        }

        // Calculate vertex
        cvar voxelA = corners[v0];
        cvar voxelB = corners[v1];

        cvar intersectionPosition = GetIntersection(voxelOffset + CellCorner[v0], voxelOffset + CellCorner[v1], voxelA.value, voxelB.value) * voxelScale;
        cvar vertexPosition = position + intersectionPosition;
        cvar voxelMaterial = GetVoxelMaterial(voxelA, voxelB);

        // Add mesh vertex
        cvar vertexInfo = VertexInfo(vertexId, vertexPosition, voxelMaterial);
        m_vertexInfo[vertexId] = vertexInfo;
        indices[i] = vertexId;
        m_vertexInfoMap.set(vertexId);

        // Add collision vertex
        collisionIndices[i] = m_collisionVertices.Count();
        m_collisionVertices.Add(vertexPosition);
    }

    cvar indexCount = cellData.GetTriangleCount() * 3;
    for (var i = 0; i < indexCount; i += 3)
    {
        cvar vertexId0 = indices[cellData.vertexIndex[i + 0]];
        cvar vertexId1 = indices[cellData.vertexIndex[i + 1]];
        cvar vertexId2 = indices[cellData.vertexIndex[i + 2]];

        rvar vertexInfoA = m_vertexInfo[vertexId0];
        rvar vertexInfoB = m_vertexInfo[vertexId1];
        rvar vertexInfoC = m_vertexInfo[vertexId2];

        cvar vertexPosition0 = vertexInfoA.vertexPosition;
        cvar vertexPosition1 = vertexInfoB.vertexPosition;
        cvar vertexPosition2 = vertexInfoC.vertexPosition;

        if (Vector3::Dot(vertexPosition0, vertexPosition1) < Math::ZeroTolerance ||
            Vector3::Dot(vertexPosition1, vertexPosition2) < Math::ZeroTolerance ||
            Vector3::Dot(vertexPosition2, vertexPosition0) < Math::ZeroTolerance)
            continue; // zero-space triangle

        cvar normal = Vector3::Cross(vertexPosition1 - vertexPosition0, vertexPosition2 - vertexPosition0);

        // Calculate normals
        vertexInfoA.vertexNormal += normal;
        vertexInfoB.vertexNormal += normal;
        vertexInfoC.vertexNormal += normal;

        vertexInfoA.normalUses += 1;
        vertexInfoB.normalUses += 1;
        vertexInfoC.normalUses += 1;

        // Add mesh triangle
        AddTriangle(vertexInfoA, vertexInfoB, vertexInfoC, normalCorrection);

        // Add collision triangle
        m_collisionIndices.Add(collisionIndices[cellData.vertexIndex[i + 0]]);
        m_collisionIndices.Add(collisionIndices[cellData.vertexIndex[i + 1]]);
        m_collisionIndices.Add(collisionIndices[cellData.vertexIndex[i + 2]]);

        // Increment triangle count
        m_triangleCount++;
    }
}
