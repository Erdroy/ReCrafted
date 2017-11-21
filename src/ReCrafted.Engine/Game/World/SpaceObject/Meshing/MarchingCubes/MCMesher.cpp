// ReCrafted © 2016-2017 Always Too Late

#include "MCMesher.h"
#include "Core/Math/Plane.h"
#include "Game/World/Voxels/VoxelUtils.h"
#include "Graphics/Mesh.h"
#include "Transvoxel.hpp"
#include "VertexCache.h"

MCMesher* MCMesher::m_instance;
const float S = 1.0f / 256.0f;

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

Vector3 Interpolate(Vector3 P0, Vector3 P1, float t)
{
	const var s = 1.0f / 256.0f;

	var u = 0x0100 - t;
	var Q = P0 * t + P1 * u;

	Q *= s;
	return Q;
}

int Sign(sbyte b)
{
	return (b >> 7) & 1;
}

Vector3 GenerateVertex(Vector3 offsetPos, Vector3 pos, float lod, float t, byte& v0, byte& v1, sbyte& d0, sbyte& d1)
{
	return pos + Interpolate((offsetPos + CornerIndex[v0]) * lod, (offsetPos + CornerIndex[v1]) * lod, t);
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

/*Vector3 Interp(Vector3 v0, Vector3 v1, Vector3 p0, Vector3 p1, sbyte* data, byte lodIndex = 0)
{
	var s0 = samples[p0];
	var s1 = samples[p1];

	var t = (s1 << 8) / (s1 - s0);
	var u = 0x0100 - t;

	if ((t & 0x00ff) == 0)
	{
		// The generated vertex lies at one of the corners so there 
		// is no need to subdivide the interval.
		if (t == 0)
		{
			return v1;
		}

		return v0;
	}
	
	for (var i = 0; i < lodIndex; ++i)
	{
		var vm = (v0 + v1) / 2;
		var pm = (p0 + p1) / 2;

		var sm = samples[pm];

		// Determine which of the sub-intervals that contain 
		// the intersection with the isosurface.
		if (Sign(s0) != Sign(sm))
		{
			v1 = vm;
			p1 = pm;
			s1 = sm;
		}
		else
		{
			v0 = vm;
			p0 = pm;
			s0 = sm;
		}
	}

	// recalculate t and u
	t = (s1 << 8) / (s1 - s0);
	u = 0x0100 - t;

	return v0 * t * S + v1 * u * S;
}*/

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

	var caseCode = ((corner[0] >> 7) & 0x01)
		| ((corner[1] >> 6) & 0x02)
		| ((corner[2] >> 5) & 0x04)
		| ((corner[3] >> 4) & 0x08)
		| ((corner[4] >> 3) & 0x10)
		| ((corner[5] >> 2) & 0x20)
		| ((corner[6] >> 1) & 0x40)
		| (corner[7] & 0x80);

	if ((caseCode ^ ((corner[7] >> 7) & 0xff)) == 0)
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

		// TODO: Vertex cache, for now - just generate it.
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
	var x = static_cast<int>(offsetPosition.x);
	var y = static_cast<int>(offsetPosition.y);
	var z = static_cast<int>(offsetPosition.z);

	static const Vector3 coords[] = {
		Vector3(0,0,0), Vector3(1,0,0), Vector3(2,0,0), // High-res lower row
		Vector3(0,1,0), Vector3(1,1,0), Vector3(2,1,0), // High-res middle row
		Vector3(0,2,0), Vector3(1,2,0), Vector3(2,2,0), // High-res upper row

		Vector3(0,0,2), Vector3(2,0,2), // Low-res lower row
		Vector3(0,2,2), Vector3(2,2,2)  // Low-res upper row
	};

	// ! BUG-GY BUG !
	// 
	//          |
	//          |
	//          |
	//         \ /

	const Vector3 pos[] = {
		(offsetPosition + coords[0x00]) , (offsetPosition + coords[0x01]) , (offsetPosition + coords[0x02]) ,
		(offsetPosition + coords[0x03]) , (offsetPosition + coords[0x04]) , (offsetPosition + coords[0x05]) ,
		(offsetPosition + coords[0x06]) , (offsetPosition + coords[0x07]) , (offsetPosition + coords[0x08]) ,
		(offsetPosition + coords[0x09]) , (offsetPosition + coords[0x0A]) ,
		(offsetPosition + coords[0x0B]) , (offsetPosition + coords[0x0C]) ,
	};

	// TODO: calculate normals

	const Vector3 samplePos[]= {
		pos[0], pos[1], pos[2],
		pos[3], pos[4], pos[5],
		pos[6], pos[7], pos[8],
		pos[0], pos[2],
		pos[6], pos[8]
	};

	sbyte samples[13];
	uint indices[15];

	for (auto i = 0; i < 13; i++) // OPTIMIZATION: Unroll
	{
		samples[i] = GetVoxel(data, samplePos[i]);
	}

	var caseCode = static_cast<uint>(Sign(samples[0]) * 0x001 |
		Sign(samples[1]) * 0x002 |
		Sign(samples[2]) * 0x004 |
		Sign(samples[5]) * 0x008 |
		Sign(samples[8]) * 0x010 |
		Sign(samples[7]) * 0x020 |
		Sign(samples[6]) * 0x040 |
		Sign(samples[3]) * 0x080 |
		Sign(samples[4]) * 0x100);

	if (caseCode == 0 || caseCode == 511)
		return;

	var classIndex = transitionCellClass[caseCode];
	var cell = &transitionCellData[classIndex & 0x7F];
	var inverse = (classIndex & 128) != 0;

	var vertexCount = cell->GetVertexCount();
	var triangleCount = cell->GetTriangleCount();

	for (var i = 0; i < vertexCount; i++)
	{
		var edgeCode = transitionVertexData[caseCode][i];

		var v1 = static_cast<byte>(edgeCode >> 4 & 0x0F);
		var v0 = static_cast<byte>(edgeCode & 0x0F);
		var lowside = (v0 > 8) && (v1 > 8);

		var d0 = GetVoxel(data, samplePos[v0]);
		var d1 = GetVoxel(data, samplePos[v1]);

		var t = (d1 << 8) / (d1 - d0);
		var u = 0x0100 - t;
		var t0 = t * S;
		var t1 = u * S;

		// TODO: vertex cache

		Vector3 vert = {};

		if ((t & 0x00ff) == 0)
		{
			// if(!present || indices[i] < 0)
			
			var p0 = pos[v0];
			var p1 = pos[v1];

			//var pi = Interp(p0, p1, samplePos[v0], samplePos[v1], samples/*, lowside ? (byte)lodIndex : (byte)(lodIndex - 1)*/);
			
			if (lowside)
			{
				// low-resolution side

				//pi[axis] = (double)origin[axis];

				//const Vector3d delta = computeDelta(pi, lodIndex, 16);
				//const Vector3d proj = projectNormal(vert.normal, delta); // TODO: we need normals

				//vert.pos[PRIMARY] = Vector3d(1000, 1000, 1000);
				//vert.pos[SECONDARY] = offset + pi + proj;
			}
			else
			{
				// high-resolution side

				//vert.pos[PRIMARY] = offset + pi;
				//vert.pos[SECONDARY] = Vector3d(1000, 1000, 1000);
			}
		}
		else
		{
			var v = t == 0 ? v1 : v0;

			var pi = pos[v];

			if (v > 8)
			{
				// low-resolution side

				//pi[axis] = (double)origin[axis];

				//const Vector3d delta = computeDelta(pi, lodIndex, 16);
				//const Vector3d proj = projectNormal(vert.normal, delta); // TODO: we need normals

				//vert.pos[PRIMARY] = Vector3d(1000, 1000, 1000);
				//vert.pos[SECONDARY] = offset + pi + proj;
			}
			else
			{
				// high-resolution side

				//vert.pos[PRIMARY] = offset + pi;
				//vert.pos[SECONDARY] = Vector3d(1000, 1000, 1000);
			}
		}

		// TODO: calculate normal

		m_vertices.add(worldPosition + vert);
		m_normals.add(Vector3::zero());
		m_uvs.add(Vector2::zero());
		m_colors.add(Vector4(85 / 255.0f, 60 / 255.0f, 50 / 255.0f, 1.0f));

		indices[i] = static_cast<uint>(m_vertices.count() - 1);
	}

	// generate triangles
	for (var t = 0; t < triangleCount; t++)
	{
		if(inverse)
		{
			m_indices.add(indices[cell->vertexIndex[t * 3 + 2]]);
			m_indices.add(indices[cell->vertexIndex[t * 3 + 1]]);
			m_indices.add(indices[cell->vertexIndex[t * 3 + 0]]);
		}
		else
		{
			m_indices.add(indices[cell->vertexIndex[t * 3 + 0]]);
			m_indices.add(indices[cell->vertexIndex[t * 3 + 1]]);
			m_indices.add(indices[cell->vertexIndex[t * 3 + 2]]);
		}
	}
}

void MCMesher::generateCells(Vector3 position, float lod)
{
	for (auto x = 0; x < SpaceObjectChunk::ChunkSize; x++)
	{
		for (auto y = 0; y < SpaceObjectChunk::ChunkSize; y++)
		{
			for (auto z = 0; z < SpaceObjectChunk::ChunkSize; z++)
			{
				var index = INDEX_3D(x, y, z, SpaceObjectChunk::ChunkSize);

				m_cells[index] = {x, y, z, -1};

				// TODO: Generate cells (mark which is transition cell etc. - edge near other chunk with higher quality)
			}
		}
	}
}

void MCMesher::polygonizeCells(Vector3 position, float lod, sbyte* data)
{
	var regularCache = new RegularCellCache();
	var transitionCache = new TransitionCellCache();

	// polygonize
	for (auto x = 0; x < SpaceObjectChunk::ChunkSize; x++)
	{
		for (auto y = 0; y < SpaceObjectChunk::ChunkSize; y++)
		{
			for (auto z = 0; z < SpaceObjectChunk::ChunkSize; z++)
			{
				const var& cell = m_cells[INDEX_3D(x, y, z, SpaceObjectChunk::ChunkSize)];

				var pos = Vector3(float(x), float(y), float(z));

				// TODO: transition cell placement (check if this is transition cell, and get it's lod levels)
				if (x == 0 || y == 0 || z == 0) // temporary
				{
					polygonizeTransitionCell(position, pos, data, lod, transitionCache);
				}
				else
				{
					polygonizeRegularCell(position, pos, data, lod, regularCache);
				}
			}
		}
	}

	SafeDelete(regularCache);
	SafeDelete(transitionCache);
}

void MCMesher::generate(Vector3 position, int lod, Ptr<Mesh>& mesh, sbyte* data)
{
	var lod_f = static_cast<float>(lod);
	m_cells = new Cell[SpaceObjectChunk::ChunkSize * SpaceObjectChunk::ChunkSize * SpaceObjectChunk::ChunkSize];

	// generate cells
	generateCells(position, lod_f);

	// polygonize cells
	polygonizeCells(position, lod_f, data);

	delete[] m_cells;

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
