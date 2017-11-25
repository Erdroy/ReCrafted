// ReCrafted © 2016-2017 Always Too Late

#include "MCMesher.h"
#include "Core/Math/Plane.h"
#include "Game/World/Voxels/VoxelUtils.h"
#include "Graphics/Mesh.h"
#include "VertexCache.h"
#include "MCTables.hpp"
#include "MSTables.hpp"

MCMesher* MCMesher::m_instance;

#define ITERATE_CELLS_BEGIN(a, b, c)\
for (auto a = 0; a < SpaceObjectChunk::ChunkSize; a++) \
for (auto b = 0; b < SpaceObjectChunk::ChunkSize; b++) \
for (auto c = 0; c < SpaceObjectChunk::ChunkSize; c++)

#define ITERATE_CELLS_END()

#define GET_CELL(_x, _y, _z) m_cells[INDEX_3D(_x, _y, _z, SpaceObjectChunk::ChunkSize)];
#define IS_BORDER(_x, _y, _z) _x == 0 || _x == SpaceObjectChunk::ChunkSize-1 || _y == 0 || _y == SpaceObjectChunk::ChunkSize-1 || _z == 0 || _z == SpaceObjectChunk::ChunkSize-1

#define BORDER_FRONT	0x01
#define BORDER_BACK		0x02
#define BORDER_LEFT		0x04
#define BORDER_RIGHT	0x08
#define BORDER_TOP		0x10
#define BORDER_BOTTOM	0x20

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
	return Vector3(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs);
}

Vector3 operator/(const Vector3& lhs, const float& rhs)
{
	return Vector3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
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
	//if (fabs(d1 - d2) < 0.0001f)
	//	return (p1 + p2) * 0.5f;

	return p1 + -d1 * (p2 - p1) / (d2 - d1);
}

inline Vector3 GetEdge(Vector3 offset, sbyte* data, Vector3 cornerA, Vector3 cornerB)
{
	var offsetA = offset + cornerA;
	var offsetB = offset + cornerB;

	// get data
	var sampleA = data[INDEX_3D(int(offsetA.x), int(offsetA.y), int(offsetA.z), SpaceObjectChunk::ChunkDataSize)] / 127.0f;
	var sampleB = data[INDEX_3D(int(offsetB.x), int(offsetB.y), int(offsetB.z), SpaceObjectChunk::ChunkDataSize)] / 127.0f;

	return GetIntersection(offsetA, offsetB, sampleA, sampleB);
}

void MCMesher::clean()
{
	// cleanup all arrays
	m_vertices.clear();
	m_normals.clear();
	m_colors.clear();
	m_uvs.clear();
	m_indices.clear();

	// NOTE: we do not need to cleanup cells as it is not required, 
	// because generating new cell overrides old data.
}

void MCMesher::generateCell(Cell* cell, int x, int y, int z, sbyte* data) const
{
	byte caseIndex = 0;

	for (auto i = 0; i < 8; i++) // TODO: unroll
	{
		if (data[INDEX_3D(x + MCCornerDeltasInt[i][0], y + MCCornerDeltasInt[i][1], z + MCCornerDeltasInt[i][2], SpaceObjectChunk::ChunkDataSize)] < ISO_LEVEL)
			caseIndex |= 1 << i;
	}

	cell->isFullOrEmpty = caseIndex == 255 || caseIndex == 0;
	cell->caseIndex = caseIndex;
}

void MCMesher::generateCube(Cell* cell, const Vector3& position, const Vector3& offset, float lod, sbyte* data)
{
	// this function is called when this cell is not full and/or empty

	for (auto i = 0; i < 15; i++) // NOTE: we can use 15 as every 16th edge case is '-1' (no vertices)
	{
		auto edge = MCEdgesTable[cell->caseIndex][i];

		// check if this case has vertices
		if (edge == -1)
			return;

		var offsetA = offset + MCEdgeOffsets[edge][0];
		var offsetB = offset + MCEdgeOffsets[edge][1];

		// get data
		var sampleA = data[INDEX_3D(int(offsetA.x), int(offsetA.y), int(offsetA.z), SpaceObjectChunk::ChunkDataSize)] / 127.0f;
		var sampleB = data[INDEX_3D(int(offsetB.x), int(offsetB.y), int(offsetB.z), SpaceObjectChunk::ChunkDataSize)] / 127.0f;

		var vertexPosition = position + GetIntersection(offsetA, offsetB, sampleA, sampleB) * lod;

		// TODO: materials support
		// TODO: vertex cache (with vertex color per material support)

		m_vertices.add(vertexPosition);
		m_indices.add(m_vertices.count() - 1);

		//cell->vertexPosition = vertexPosition;
		//cell->vertexIndex = m_vertices.count() - 1;
	}
}

void MCMesher::generateSkirt(const Vector3& position, const Vector3& offset, float lod, uint8_t axis, sbyte* data)
{
	var corners = MSCornerOffsets[axis];

	byte caseIndex = 0;
	Vector3 edges[8];

	for (auto i = 0; i < 4; i++) // TODO: unroll
	{
		if (data[INDEX_3D(
			static_cast<int>(offset.x + corners[i].x),
			static_cast<int>(offset.y + corners[i].y),
			static_cast<int>(offset.z + corners[i].z),
			SpaceObjectChunk::ChunkDataSize)] < ISO_LEVEL)
		{
			caseIndex |= 1 << i;
		}
	}

	edges[0] = offset + corners[0];
	edges[2] = offset + corners[1];
	edges[4] = offset + corners[2];
	edges[6] = offset + corners[3];

	edges[1] = GetEdge(offset, data, corners[0], corners[1]);
	edges[3] = GetEdge(offset, data, corners[1], corners[2]);
	edges[5] = GetEdge(offset, data, corners[2], corners[3]);
	edges[7] = GetEdge(offset, data, corners[3], corners[0]);

	for (var i = 0; i < 15; i++)
	{
		var edge = MSEdgesTable[caseIndex][i];

		if (edge == -1)
			return;

		var vertexPosition = position + edges[edge] * lod;

		m_vertices.add(vertexPosition);
		m_indices.add(m_vertices.count() - 1);
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

			if (borders > 0)
			{
				if (z == SpaceObjectChunk::ChunkSize - 1) // AXIS_FRONT
				{
					generateSkirt(position, offset, lod, AXIS_FRONT, data);
				}
				if(z == 0) // AXIS_BACK
				{
					generateSkirt(position, offset, lod, AXIS_BACK, data);
				}
				if (x == 0) // AXIS_BACK
				{
					generateSkirt(position, offset, lod, AXIS_LEFT, data);
				}
				if (x == SpaceObjectChunk::ChunkSize - 1) // AXIS_RIGHT
				{
					generateSkirt(position, offset, lod, AXIS_RIGHT, data);
				}
				if (y == SpaceObjectChunk::ChunkSize - 1) // AXIS_TOP
				{
					generateSkirt(position, offset, lod, AXIS_TOP, data);
				}
				if (y == 0) // AXIS_BOTTOM
				{
					generateSkirt(position, offset, lod, AXIS_BOTTOM, data);
				}
			}
		}
	}
	ITERATE_CELLS_END()
}

void MCMesher::generate(const Vector3& position, int lod, Ptr<Mesh>& mesh, sbyte* data)
{
	var lodF = static_cast<float>(lod);

	// TODO: select which borders need skirts

		generateCells(data, position, lodF, 1);
	
	if (m_vertices.count() == 0)
	{
		// cleanup
		clean();
		return;
	}

	// Calculate normals
	for (var i = 0u; i < m_vertices.count(); i += 3)
	{
		var p0 = m_vertices[i + 0];
		var p1 = m_vertices[i + 1];
		var p2 = m_vertices[i + 2];

		var plane = Plane(p0, p1, p2);
		plane.normalize();

		m_normals.add(plane.normal);
		m_normals.add(plane.normal);
		m_normals.add(plane.normal);

		m_uvs.add(Vector2::zero());
		m_uvs.add(Vector2::zero());
		m_uvs.add(Vector2::zero());

		m_colors.add(Vector4(85 / 255.0f, 60 / 255.0f, 50 / 255.0f, 1.0f));
		m_colors.add(Vector4(85 / 255.0f, 60 / 255.0f, 50 / 255.0f, 1.0f));
		m_colors.add(Vector4(85 / 255.0f, 60 / 255.0f, 50 / 255.0f, 1.0f));
	}

	mesh->setVertices(m_vertices.data(), m_vertices.count());
	mesh->setUVs(m_uvs.data());
	mesh->setNormals(m_normals.data());
	mesh->setColors(m_colors.data());
	mesh->setIndices(m_indices.data(), m_indices.count());

	mesh->applyChanges();

	// cleanup
	clean();
}
