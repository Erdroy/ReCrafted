// ReCrafted © 2016-2017 Always Too Late

#include "MCMesher.h"
#include "Graphics/Mesh.h"
#include "Transvoxel.h"
#include "Game/World/Voxels/VoxelUtils.h"
#include "Core/Math/Plane.h"

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

Vector3 InterpolateVoxelVector(long t, Vector3 P0, Vector3 P1)
{
	const var s = 1.0f / 256.0f;

	var u = 0x0100 - t;
	var Q = P0 * t + P1 * u;
	Q *= s;
	return Q;
}

Vector3 GenerateVertex(Vector3 offsetPos, Vector3 pos, int lod, long t, byte& v0, byte& v1, sbyte& d0, sbyte& d1)
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

void MCMesher::generate(Vector3 position, float sizeMod, Ptr<Mesh>& mesh, sbyte* data, uint16_t count)
{
	var lod = static_cast<int>(sizeMod);

	sbyte corner[8];
	uint indices[15];
	Vector3 cornerNormals[8];

	for (auto x = 0; x < count; x++)
	{
		for (auto y = 0; y < count; y++)
		{
			for (auto z = 0; z < count; z++)
			{
				var pos = Vector3(float(x), float(y), float(z));

				for (auto i = 0; i < 8; i++) // OPTIMIZATION: Unroll
				{
					corner[i] = GetVoxel(data, x + CornerIndexInt[i][0], y + CornerIndexInt[i][1], z + CornerIndexInt[i][2]);
				}
				
				unsigned long caseCode = getCaseCode(corner, 8);

				if ((caseCode ^ ((corner[7] >> 7) & 0xFF)) == 0)
					continue;

				for (var i = 0; i < 8; i++)
				{
					var p = pos + CornerIndex[i];
					var nx = (GetVoxel(data, p + Vector3(1.0f, 0.0f, 0.0f)) - GetVoxel(data, p - Vector3(1.0f, 0.0f, 0.0f))) * 0.5f;
					var ny = (GetVoxel(data, p + Vector3(0.0f, 1.0f, 0.0f)) - GetVoxel(data, p - Vector3(0.0f, 1.0f, 0.0f))) * 0.5f;
					var nz = (GetVoxel(data, p + Vector3(0.0f, 0.0f, 1.0f)) - GetVoxel(data, p - Vector3(0.0f, 0.0f, 1.0f))) * 0.5f;

					cornerNormals[i].x = nx;
					cornerNormals[i].y = ny;
					cornerNormals[i].z = nz;
					cornerNormals[i].normalize();
				}

				var vertexData = regularVertexData[caseCode];
				var cellClass = regularCellClass[caseCode];

				var cell = &regularCellData[cellClass];

				var vertexCount = cell->GetVertexCount();
				var triangleCount = cell->GetTriangleCount();

				for(var i = 0; i < vertexCount; i ++)
				{
					//var edge = static_cast<byte>(vertexData[i] >> 8);
					//var reuseIndex = static_cast<byte>(edge & 0xF);
					//var rDir = static_cast<byte>(edge >> 4);

					var v1 = static_cast<byte>(vertexData[i] & 0x0F);
					var v0 = static_cast<byte>(vertexData[i] >> 4 & 0x0F);

					var d0 = corner[v0];
					var d1 = corner[v1];

					var t = (d1 << 8) / (d1 - d0);
					var u = 0x0100 - t;
					var t0 = t / 256.0f;
					var t1 = u / 256.0f;

					var index = -1;

					// TODO: vertex reuse

					if (index == -1)
					{
						var vertexPosition = GenerateVertex(pos, position, lod, t, v0, v1, d0, d1);

						m_vertices.add(vertexPosition);
						m_normals.add(cornerNormals[v0] * t0 + cornerNormals[v1] * t1);
						m_uvs.add(Vector2::zero());
						m_colors.add(Vector4(85 / 255.0f, 60 / 255.0f, 50 / 255.0f, 1.0f));

						index = m_vertices.count() - 1;
					}

					indices[i] = static_cast<uint>(index);
				}

				for (var t = 0; t < triangleCount; t++)
				{
					m_indices.add(indices[cell->vertexIndex[t * 3 + 0]]);
					m_indices.add(indices[cell->vertexIndex[t * 3 + 1]]);
					m_indices.add(indices[cell->vertexIndex[t * 3 + 2]]);
				}
			}
		}
	}

	if (m_indices.count() == 0 || m_vertices.count() == 0)
		return;

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
