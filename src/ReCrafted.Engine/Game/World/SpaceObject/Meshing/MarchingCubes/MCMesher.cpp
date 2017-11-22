// ReCrafted © 2016-2017 Always Too Late

#include "MCMesher.h"
#include "Core/Math/Plane.h"
#include "Game/World/Voxels/VoxelUtils.h"
#include "Graphics/Mesh.h"
#include "VertexCache.h"
#include "MCTables.hpp"

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

void MCMesher::generate(Vector3 position, int lod, Ptr<Mesh>& mesh, sbyte* data)
{
	var lod_f = static_cast<float>(lod);

	const var count = SpaceObjectChunk::ChunkDataSize;

	auto vertexIndex = 0u;
	for (auto y = 0; y < count - 1; y++)
	{
		for (auto x = 0; x < count - 1; x++)
		{
			for (auto z = 0; z < count - 1; z++)
			{
				var pos = Vector3(float(x), float(y), float(z));

				byte corners = 0;
				for (auto i = 0; i < 8; i++)
				{
					if (data[INDEX_3D(x + CornerDeltasInt[i][0], y + CornerDeltasInt[i][1], z + CornerDeltasInt[i][2], count)] < 0)
						corners |= (1 << i);
				}

				if (corners == 0 || corners == 255)
					continue;

				var edgeCase = 0;

				for (auto i = 0; i < 15; i++)
				{
					auto edge = EdgesTable[corners][edgeCase];

					if (edge == -1)
						break;

					var offsetA = pos + EdgeVertexOffsets[edge][0];
					var offsetB = pos + EdgeVertexOffsets[edge][1];

					var sampleA = data[INDEX_3D(int(offsetA.x), int(offsetA.y), int(offsetA.z), count)] / 127.0f;
					var sampleB = data[INDEX_3D(int(offsetB.x), int(offsetB.y), int(offsetB.z), count)] / 127.0f;

					var vertexPosition = position + GetIntersection(offsetA, offsetB, sampleA, sampleB) * lod_f;

					m_vertices.add(vertexPosition);
					m_indices.add(vertexIndex);

					vertexIndex++;
					edgeCase++;
				}
			}
		}
	}

	if (vertexIndex == 0)
		return;

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

	m_vertices.clear();
	m_normals.clear();
	m_colors.clear();
	m_uvs.clear();
	m_indices.clear();
}
