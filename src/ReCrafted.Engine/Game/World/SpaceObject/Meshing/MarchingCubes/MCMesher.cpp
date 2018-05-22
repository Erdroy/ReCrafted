// ReCrafted (c) 2016-2018 Always Too Late

#include "MCMesher.h"
#include "Core/Math/Plane.h"
#include "Graphics/Mesh.h"
#include "../../Utilities/VoxelUtils.h"

#include "MCTables.hpp"
#include "MSTables.hpp"

#define ITERATE_CELLS_BEGIN(a, b, c)\
for (auto a = 0; a < VoxelChunkData::ChunkSize; a++) \
for (auto b = 0; b < VoxelChunkData::ChunkSize; b++) \
for (auto c = 0; c < VoxelChunkData::ChunkSize; c++)

#define ITERATE_CELLS_END()

#define GET_CELL(_x, _y, _z) m_cells[INDEX_3D(_x, _y, _z, VoxelChunkData::ChunkSize)];
#define IS_BORDER(_x, _y, _z) _x == 0 || _x == VoxelChunkData::ChunkSize-1 || _y == 0 || _y == VoxelChunkData::ChunkSize-1 || _z == 0 || _z == VoxelChunkData::ChunkSize-1

#define AXIS_FRONT		0
#define AXIS_BACK		1
#define AXIS_LEFT		2
#define AXIS_RIGHT		3
#define AXIS_TOP		4
#define AXIS_BOTTOM		5

const int ISO_LEVEL = 0;
const float S = 1.0f / 256.0f;

// TODO: move operator overrides into Vector3
Vector3 operator*(const float& lhs, const Vector3& rhs)
{
    return {rhs.x * lhs, rhs.y * lhs, rhs.z * lhs};
}

Vector3 operator/(const Vector3& lhs, const float& rhs)
{
    return {lhs.x / rhs, lhs.y / rhs, lhs.z / rhs};
}

/**
 * \brief Calculates surface intersection on edge based on two data samples.
 * \param p1 The first point.
 * \param p2 The second point.
 * \param d1 The first point value.
 * \param d2 The second point value.
 * \return The intersection of the given points.
 */
inline Vector3 GetIntersection(Vector3& p1, Vector3& p2, float d1, float d2)
{
    return p1 + -d1 * (p2 - p1) / (d2 - d1);
}

inline Vector3 GetEdge(Vector3 offset, sbyte* data, Vector3 cornerA, Vector3 cornerB)
{
    var offsetA = offset + cornerA;
    var offsetB = offset + cornerB;

    // get data
    cvar sampleA = VOXEL_TO_FLOAT(data[INDEX_3D(int(offsetA.x), int(offsetA.y), int(offsetA.z), VoxelChunkData::ChunkDataSize)]);
    cvar sampleB = VOXEL_TO_FLOAT(data[INDEX_3D(int(offsetB.x), int(offsetB.y), int(offsetB.z), VoxelChunkData::ChunkDataSize)]);

    return GetIntersection(offsetA, offsetB, sampleA, sampleB);
}

inline float GetVoxel(sbyte* data, const Vector3& point)
{
    return VOXEL_TO_FLOAT(data[INDEX_3D(int(point.x), int(point.y), int(point.z), VoxelChunkData::ChunkDataSize)]);
}

void MCMesher::clean()
{
    // cleanup all arrays
    m_vertices.clear();
    m_normals.clear();
    m_colors.clear();
    m_indices.clear();

    // NOTE: we do not need to cleanup cells as it is not required, 
    // because generating new cell overrides old data.
}

void MCMesher::generateCell(Cell* cell, int x, int y, int z, sbyte* data) const
{
    byte caseIndex = 0;

    for (auto i = 0; i < 8; i++) // TODO: unroll
    {
        if (data[INDEX_3D(x + MCCornerDeltasInt[i][0], y + MCCornerDeltasInt[i][1], z + MCCornerDeltasInt[i][2],
            VoxelChunkData::ChunkDataSize)] > ISO_LEVEL)
            caseIndex |= 1 << i;
    }

    cell->isFullOrEmpty = caseIndex == 255 || caseIndex == 0;
    cell->caseIndex = caseIndex;
}

void MCMesher::generateCube(Cell* cell, const Vector3& position, const Vector3& offset, float lod, sbyte* data)
{
    // this function is called when this cell is not full and/or empty

    cvar startIndex = m_vertices.count();
    var triangleCount = 0;

    for (auto i = 0; i < 15; i++)
    {
        cvar edge = MCEdgesTable[cell->caseIndex][i];

        // check if this case has vertices
        if (edge == -1)
            break;

        var offsetA = offset + MCEdgeOffsets[edge][0];
        var offsetB = offset + MCEdgeOffsets[edge][1];

        // get data
        cvar sampleA = GetVoxel(data, offsetA);
        cvar sampleB = GetVoxel(data, offsetB);

        cvar vertexPosition = position + GetIntersection(offsetA, offsetB, sampleA, sampleB) * lod;

        // TODO: materials support
        // TODO: vertex cache (with vertex color per material support)
        // TODO: fix issue with 2 vertices (invalid normals)

        m_vertices.add(vertexPosition);
        m_indices.add(startIndex + i);

        triangleCount++;
    }

    if (triangleCount == 0)
        return;

    // generate normals
    var normal = Vector3::zero();
    var triangles = 0;
    for (var i = 0; i < triangleCount; i += 3)
    {
        cvar p0 = m_vertices[m_indices[startIndex + i + 0]];
        cvar p1 = m_vertices[m_indices[startIndex + i + 1]];
        cvar p2 = m_vertices[m_indices[startIndex + i + 2]];

        cvar plane = Plane(p2, p1, p0);

        m_colors.add(Vector4(0.35f, 0.35f, 0.35f, 1.0f));
        m_colors.add(Vector4(0.35f, 0.35f, 0.35f, 1.0f));
        m_colors.add(Vector4(0.35f, 0.35f, 0.35f, 1.0f));

        m_normals.add(plane.normal);
        m_normals.add(plane.normal);
        m_normals.add(plane.normal);

        normal += plane.normal;
        triangles++;
    }

    if (triangles <= 1)
        return;

    // smooth the cell normal
    cell->vertexNormal = normal / static_cast<float>(triangles);
    cell->vertexNormal.normalize();
}

void MCMesher::generateSkirt(Cell* cell, const Vector3& position, const Vector3& offset, float lod, uint8_t axis,
                             sbyte* data)
{
    cvar corners = MSCornerOffsets[axis];

    byte caseIndex = 0;
    Vector3 edges[8];

    for (auto i = 0; i < 4; i++) // TODO: unroll
    {
        if (GetVoxel(data, offset + corners[i]) < ISO_LEVEL)
            caseIndex |= 1 << i;
    }

    edges[0] = offset + corners[0];
    edges[2] = offset + corners[1];
    edges[4] = offset + corners[2];
    edges[6] = offset + corners[3];

    edges[1] = GetEdge(offset, data, corners[0], corners[1]);
    edges[3] = GetEdge(offset, data, corners[1], corners[2]);
    edges[5] = GetEdge(offset, data, corners[2], corners[3]);
    edges[7] = GetEdge(offset, data, corners[3], corners[0]);

    for (var i = 0; i < 12; i++)
    {
        cvar edge = MSEdgesTable[caseIndex][i];

        if (edge == -1)
            return;

        cvar vertexPosition = position + edges[edge] * lod;

        m_vertices.add(vertexPosition);
        m_indices.add(m_vertices.count() - 1);

        m_colors.add(Vector4(0.35f, 0.35f, 0.35f, 1.0f));
        m_normals.add(cell->vertexNormal);
    }
}

void MCMesher::generateCells(sbyte* data, const Vector3& position, float lod, uint8_t borders)
{
    // generate all cells
    ITERATE_CELLS_BEGIN(x, y, z)
    {
        const var offset = Vector3(float(x), float(y), float(z));
        const var cell = &GET_CELL(x, y, z);

        // generate cell data
        generateCell(cell, x, y, z, data);

        // generate cube mesh
        if (!cell->isFullOrEmpty)
        {
            generateCube(cell, position, offset, lod, data);

            // generate skirt for this cell
            if (borders > 0)
            {
                // TODO: check if skirt can be generated (if `borders` has the axis flag)
                // AXIS_FRONT
                if (z == VoxelChunkData::ChunkSize - 1 && (borders & BORDER_FRONT) != 0)
                {
                    generateSkirt(cell, position, offset, lod, AXIS_FRONT, data);
                }
                // AXIS_BACK
                if (z == 0 && (borders & BORDER_BACK) != 0)
                {
                    generateSkirt(cell, position, offset, lod, AXIS_BACK, data);
                }
                // AXIS_BACK
                if (x == 0 && (borders & BORDER_LEFT) != 0)
                {
                    generateSkirt(cell, position, offset, lod, AXIS_LEFT, data);
                }
                // AXIS_RIGHT
                if (x == VoxelChunkData::ChunkSize - 1 && (borders & BORDER_RIGHT) != 0)
                {
                    generateSkirt(cell, position, offset, lod, AXIS_RIGHT, data);
                }
                // AXIS_TOP
                if (y == VoxelChunkData::ChunkSize - 1 && (borders & BORDER_TOP) != 0)
                {
                    generateSkirt(cell, position, offset, lod, AXIS_TOP, data);
                }
                // AXIS_BOTTOM
                if (y == 0 && (borders & BORDER_BOTTOM) != 0)
                {
                    generateSkirt(cell, position, offset, lod, AXIS_BOTTOM, data);
                }
            }
        }
    }
    ITERATE_CELLS_END()
}

void MCMesher::generate(const Vector3& position, int lod, uint8_t borders, Ref<Mesh>& mesh, sbyte* data)
{
    cvar lodF = static_cast<float>(lod);

    generateCells(data, position, lodF, lod > 1 ? borders : 0);

    if (m_vertices.count() < 3 || m_indices.count() < 3)
    {
        // cleanup
        clean();
        return;
    }

    mesh->setVertices(m_vertices.data(), m_vertices.count());
    mesh->setNormals(m_normals.data());
    mesh->setColors(m_colors.data());
    mesh->setIndices(m_indices.data(), m_vertices.count());

    // apply
    mesh->applyChanges();

    // cleanup
    clean();
}
