// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#if 0
#include "MarchingCubesMesher.h"
#include "MarchingCubesTables.hpp"
#include "MarchingSquaresTables.hpp"

#include "Core/Math/Plane.h"
#include "Graphics/Mesh.h"

#include "../CommonTables.hpp"
#include "../MeshingHelpers.h"
#include "../../Utilities/VoxelUtils.h"

Int3 MSCellCorner[8] = {
    Int3(0, 0, 0),
    Int3(1, 0, 0),
    Int3(1, 1, 0),
    Int3(0, 1, 0),
    Int3(0, 0, 1),
    Int3(1, 0, 1),
    Int3(1, 1, 1),
    Int3(0, 1, 1)
};

#define ITERATE_CELLS_BEGIN(a, b, c)\
for (auto a = VoxelChunkData::ChunkDataStart; a < VoxelChunkData::ChunkSize; a++) {\
for (auto b = VoxelChunkData::ChunkDataStart; b < VoxelChunkData::ChunkSize; b++) {\
for (auto c = VoxelChunkData::ChunkDataStart; c < VoxelChunkData::ChunkSize; c++) {

#define ITERATE_CELLS_END() }}}

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

inline Vector3 GetEdge(Vector3 offset, Voxel* data, Vector3 cornerA, Vector3 cornerB)
{
    var offsetA = offset + cornerA;
    var offsetB = offset + cornerB;

    // get data
    cvar sampleA = VOXEL_TO_FLOAT(GetVoxel(data, offsetA).value);
    cvar sampleB = VOXEL_TO_FLOAT(GetVoxel(data, offsetB).value);

    return GetIntersection(offsetA, offsetB, sampleA, sampleB);
}

void MCMesher::GenerateCell(Cell* cell, const Int3& offset, Voxel* data) const
{
    byte caseIndex = 0;

    for (auto i = 0; i < 8; i++) // TODO: unroll
    {
        if (GetVoxel(data, offset + MSCellCorner[i]).value > ISO_LEVEL)
            caseIndex |= 1 << i;
    }

    cell->isFullOrEmpty = caseIndex == 255 || caseIndex == 0;
    cell->caseIndex = caseIndex;
}

void MCMesher::GenerateCube(Cell* cell, const Vector3& position, const Vector3& offset, float lod, Voxel* data)
{
    // this function is called when this cell is not full and/or empty

    cvar startIndex = m_vertices.Count();
    var triangleCount = 0;

    for (auto i = 0; i < 15; i++)
    {
        cvar edge = MCEdgesTable[cell->caseIndex][i];

        // check if this case has vertices
        if (edge == -1)
            break;

        cvar vertexPosition = position + GetEdge(offset, data, MCEdgeOffsets[edge][0], MCEdgeOffsets[edge][1]) * lod;

        // TODO: materials support
        // TODO: vertex cache (with vertex color per material support)
        // TODO: fix issue with 2 vertices (invalid normals)

        m_vertices.Add(vertexPosition);
        m_indices.Add(startIndex + i);

        triangleCount++;
    }

    if (triangleCount == 0)
        return;

    // generate normals
    var normal = Vector3::Zero();
    var triangles = 0;
    for (var i = 0; i < triangleCount; i += 3)
    {
        cvar p0 = m_vertices[m_indices[startIndex + i + 0]];
        cvar p1 = m_vertices[m_indices[startIndex + i + 1]];
        cvar p2 = m_vertices[m_indices[startIndex + i + 2]];

        cvar plane = Plane(p2, p1, p0);

        m_colors.Add(Vector4(0.35f, 0.35f, 0.35f, 1.0f));
        m_colors.Add(Vector4(0.35f, 0.35f, 0.35f, 1.0f));
        m_colors.Add(Vector4(0.35f, 0.35f, 0.35f, 1.0f));

        m_normals.Add(plane.normal);
        m_normals.Add(plane.normal);
        m_normals.Add(plane.normal);

        normal += plane.normal;
        triangles++;
    }

    if (triangles <= 1)
        return;

    // smooth the cell normal
    cell->vertexNormal = normal / static_cast<float>(triangles);
    cell->vertexNormal.Normalize();
}

void MCMesher::GenerateSkirt(Cell* cell, const Vector3& position, const Vector3& offset, float lod, uint8_t axis, Voxel* data)
{
    cvar corners = MSCornerOffsets[axis];

    byte caseIndex = 0;
    Vector3 edges[8];

    for (auto i = 0; i < 4; i++) // TODO: unroll
    {
        if (GetVoxel(data, offset + corners[i]).value < ISO_LEVEL)
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

        m_vertices.Add(vertexPosition);
        m_indices.Add(m_vertices.Count() - 1);

        m_colors.Add(Vector4(0.35f, 0.35f, 0.35f, 1.0f));
        m_normals.Add(cell->vertexNormal);
    }
}

void MCMesher::GenerateCells(Voxel* data, const Vector3& position, float lod, uint8_t borders)
{
    // generate all cells
    ITERATE_CELLS_BEGIN(x, y, z)
    {
        if (x < 0 || y < 0 || z < 0)
            continue; // Ignore normal correction (we don't have vertex reuse here)

        cvar offset = Vector3(float(x), float(y), float(z));
        cvar offsetInt3 = Int3(x, y, z);
        cvar cell = &GET_CELL(x, y, z);

        // generate cell data
        GenerateCell(cell, offsetInt3, data);

        // generate cube mesh
        if (!cell->isFullOrEmpty)
        {
            GenerateCube(cell, position, offset, lod, data);

            // generate skirt for this cell
            if (borders > 0 && lod > 0)
            {
                // AXIS_FRONT
                if (z == VoxelChunkData::ChunkSize - 1 && borders & 1 << AXIS_FRONT)
                {
                    GenerateSkirt(cell, position, offset, lod, AXIS_FRONT, data);
                }

                // AXIS_BACK
                if (z == 0 && borders & 1 << AXIS_BACK)
                {
                    GenerateSkirt(cell, position, offset, lod, AXIS_BACK, data);
                }

                // AXIS_LEFT
                if (x == 0 && borders & 1 << AXIS_LEFT)
                {
                    GenerateSkirt(cell, position, offset, lod, AXIS_LEFT, data);
                }

                // AXIS_RIGHT
                if (x == VoxelChunkData::ChunkSize - 1 && borders & 1 << AXIS_RIGHT)
                {
                    GenerateSkirt(cell, position, offset, lod, AXIS_RIGHT, data);
                }

                // AXIS_TOP
                if (y == VoxelChunkData::ChunkSize - 1 && borders & 1 << AXIS_TOP)
                {
                    GenerateSkirt(cell, position, offset, lod, AXIS_TOP, data);
                }

                // AXIS_BOTTOM
                if (y == 0 && borders & 1 << AXIS_BOTTOM)
                {
                    GenerateSkirt(cell, position, offset, lod, AXIS_BOTTOM, data);
                }
            }
        }
    }
    ITERATE_CELLS_END()
}

void MCMesher::Apply(const RefPtr<Mesh>& mesh)
{
    mesh->SetVertices(m_vertices.Data(), m_vertices.Count());
    mesh->SetNormals(m_normals.Data());
    mesh->SetColors(m_colors.Data());
    mesh->SetIndices(m_indices.Data(), m_vertices.Count());

    mesh->ApplyChanges();

    Clear();
}

void MCMesher::Generate(const Vector3& position, int lod, uint8_t borders, Voxel* data)
{
    cvar lodF = static_cast<float>(lod);

    GenerateCells(data, position, lodF, lod > 1 ? borders : 0);

    if (m_vertices.Count() < 3 || m_indices.Count() < 3)
        Clear();
}

void MCMesher::Clear()
{
    // cleanup all arrays
    m_vertices.Clear();
    m_normals.Clear();
    m_colors.Clear();
    m_indices.Clear();

    // NOTE: we do not need to cleanup cells as it is not required, 
    // because generating new cell overrides old data.
}
#endif
