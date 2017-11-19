// ReCrafted © 2016-2017 Always Too Late

#include "MCMesher.h"
#include "Core/Math/Plane.h"
#include "Game/World/Voxels/VoxelUtils.h"
#include "Graphics/Mesh.h"
#include "Transvoxel.hpp"
#include "VertexCache.h"

MCMesher* MCMesher::m_instance;

Vector3 operator*(const float& lhs, const Vector3& rhs)
{
	return Vector3(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs);
}

Vector3 operator/(const Vector3& lhs, const float& rhs)
{
	return Vector3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}

Vector3 GetIntersection(Vector3& p1, Vector3& p2, float d1, float d2)
{
	return p1 + -d1 * (p2 - p1) / (d2 - d1);
}

Vector3 InterpolateVoxelVector(float t, Vector3 P0, Vector3 P1)
{
	const var s = 1.0f / 256.0f;

	var u = 0x0100 - t;
	var Q = P0 * t + P1 * u;
	Q *= s;
	return Q;
}

Vector3 GenerateVertex(Vector3 offsetPos, Vector3 pos, float lod, float t, byte& v0, byte& v1, sbyte& d0, sbyte& d1)
{
	return pos + InterpolateVoxelVector(t, (offsetPos + CornerIndex[v0]) * lod, (offsetPos + CornerIndex[v1]) * lod);
}

sbyte GetVoxel(sbyte* data, int x, int y, int z)
{
	return data[INDEX_3D(1 + x, 1 + y, 1 + z, SpaceObjectChunk::ChunkDataSize)];
}

sbyte GetVoxel(sbyte* data, Vector3 pos)
{
	var x = static_cast<int>(pos.x);
	var y = static_cast<int>(pos.y);
	var z = static_cast<int>(pos.z);

	return GetVoxel(data, x, y, z);
}

inline Vector3 prevOffset(uint8_t dir)
{
	return Vector3(-(dir & 1),
		-((dir >> 1) & 1),
		-((dir >> 2) & 1));
}

byte getCaseCode(sbyte* density, int densityLength)
{
	byte code = 0;
	byte konj = 0x01;
	for (int i = 0; i < densityLength; i++)
	{
		code |= (byte)((density[i] >> (densityLength - 1 - i)) & konj);
		konj <<= 1;
	}

	return code;
}

void MCMesher::polygonizeRegularCell(Vector3 worldPosition, Vector3 offsetPosition, sbyte* data, float lod, RegularCellCache* cache)
{
	var directionMask = static_cast<byte>((offsetPosition.x > 0 ? 1 : 0) | ((offsetPosition.z > 0 ? 1 : 0) << 1) | ((offsetPosition.y > 0 ? 1 : 0) << 2));

	sbyte corner[8];
	uint indices[15];
	Vector3 cornerNormals[8];

	for (auto i = 0; i < 8; i++) // OPTIMIZATION: Unroll
	{
		corner[i] = GetVoxel(data, offsetPosition + Vector3(CornerIndexInt[i][0], CornerIndexInt[i][1], CornerIndexInt[i][2]));
	}

	unsigned long caseCode = getCaseCode(corner, 8);

	if ((caseCode ^ ((corner[7] >> 7) & 0xFF)) == 0)
		return;

	for (var i = 0; i < 8; i++)
	{
		var p = offsetPosition + CornerIndex[i];

		cornerNormals[i].x = (GetVoxel(data, p + Vector3(1.0f, 0.0f, 0.0f)) - GetVoxel(data, p - Vector3(1.0f, 0.0f, 0.0f))) * 0.5f;
		cornerNormals[i].y = (GetVoxel(data, p + Vector3(0.0f, 1.0f, 0.0f)) - GetVoxel(data, p - Vector3(0.0f, 1.0f, 0.0f))) * 0.5f;
		cornerNormals[i].z = (GetVoxel(data, p + Vector3(0.0f, 0.0f, 1.0f)) - GetVoxel(data, p - Vector3(0.0f, 0.0f, 1.0f))) * 0.5f;
		cornerNormals[i].normalize();

		cornerNormals[i] *= -1.0f;
	}

	var vertexData = regularVertexData[caseCode];
	var cellClass = regularCellClass[caseCode];
	var cell = &regularCellData[cellClass];

	var vertexCount = cell->GetVertexCount();
	var triangleCount = cell->GetTriangleCount();

	for (var i = 0; i < vertexCount; i++)
	{
		var v1 = static_cast<byte>(vertexData[i] & 0x0F);
		var v0 = static_cast<byte>(vertexData[i] >> 4 & 0x0F);

		var d0 = corner[v0];
		var d1 = corner[v1];

		var t = static_cast<float>((d1 << 8) / (d1 - d0));
		var u = 0x0100 - t;
		var t0 = t / 256.0f;
		var t1 = u / 256.0f;

		// no cached vertex found, generate new one
		var vertexPosition = GenerateVertex(offsetPosition, worldPosition, static_cast<float>(lod), t, v0, v1, d0, d1);

		m_vertices.add(vertexPosition);
		m_normals.add(cornerNormals[v0] * t0 + cornerNormals[v1] * t1);
		m_uvs.add(Vector2::zero());
		m_colors.add(Vector4(85 / 255.0f, 60 / 255.0f, 50 / 255.0f, 1.0f));

		indices[i] = static_cast<uint>(m_vertices.count() - 1);
	}

	for (var t = 0; t < triangleCount; t++)
	{
		m_indices.add(indices[cell->vertexIndex[t * 3 + 0]]);
		m_indices.add(indices[cell->vertexIndex[t * 3 + 1]]);
		m_indices.add(indices[cell->vertexIndex[t * 3 + 2]]);
	}
}

void MCMesher::polygonizeTransitionCell(Vector3 worldPosition, Vector3 offsetPosition, sbyte* data, float lod, TransitionCellCache* cache)
{
	// TODO: transition cell implementation
}

void MCMesher::generate(Vector3 position, int lod, Ptr<Mesh>& mesh, sbyte* data)
{
	var lod_f = static_cast<float>(lod);

	var regularCache = new RegularCellCache();
	//var transitionCache = new TransitionCellCache();

	for (auto x = 0; x < SpaceObjectChunk::ChunkSize; x++)
	{
		for (auto y = 0; y < SpaceObjectChunk::ChunkSize; y++)
		{
			for (auto z = 0; z < SpaceObjectChunk::ChunkSize; z++)
			{
				var pos = Vector3(float(x), float(y), float(z));

				polygonizeRegularCell(position, pos, data, lod_f, regularCache);

				// how the fuck I can apply transition cell here? Check neigh, or what? TODO: transition cell placement
			}
		}
	}

	SafeDelete(regularCache);
	//SafeDelete(transitionCache);

	if (m_indices.count() == 0 || m_vertices.count() == 0)
		return;

	// TODO: Try using mesh simplification

	mesh->setVertices(m_vertices.data(), m_vertices.count());
	mesh->setUVs(m_uvs.data());
	mesh->setNormals(m_normals.data());
	mesh->setColors(m_colors.data());
	mesh->setIndices(m_indices.data(), m_indices.count());

	mesh->applyChanges();

	m_vertices.clear();
	m_normals.clear();
	m_colors.clear();
	m_uvs.clear();
	m_indices.clear();
}
