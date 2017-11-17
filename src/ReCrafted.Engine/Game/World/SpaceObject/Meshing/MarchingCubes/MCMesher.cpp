// ReCrafted © 2016-2017 Always Too Late

#include "MCMesher.h"
#include "Graphics/Mesh.h"
#include "MCTables.h"
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

void MCMesher::generate(Vector3 position, float sizeMod, Ptr<Mesh>& mesh, sbyte* data, uint16_t count)
{
	sbyte corner[8];

	auto vertexIndex = 0u;
	for (auto y = 0; y < count - 1; y++) // NOTE: xy|z for vertex sharing
	{
		for (auto x = 0; x < count - 1; x++)
		{
			for (auto z = 0; z < count - 1; z++)
			{
				auto pos = Vector3(float(x), float(y), float(z));

				for (auto i = 0; i < 8; i++) // OPTIMIZATION: Unroll
				{
					corner[i] = data[INDEX_3D(x + CornerDeltasInt[i][0], y + CornerDeltasInt[i][1], z + CornerDeltasInt[i][2], count)];
				}
				
				unsigned long caseCode = ((corner[0] >> 7) & 0x01)
					| ((corner[1] >> 6) & 0x02)
					| ((corner[2] >> 5) & 0x04)
					| ((corner[3] >> 4) & 0x08)
					| ((corner[4] >> 3) & 0x10)
					| ((corner[5] >> 2) & 0x20)
					| ((corner[6] >> 1) & 0x40)
					| (corner[7] & 0x80);

				if ((caseCode ^ ((corner[7] >> 7) & 0xFF)) == 0)
					continue;

				auto vertexData = regularVertexData[caseCode];


				auto edgeCase = 0;

				for (auto i = 0; i < 15; i++)
				{
					auto edge = &regularCellData[edgeCase];

					if (edge->GetVertexCount() == 0)
						break;
					
					auto edgeInfo = vertexData[];

					// TODO: get low byte and the vertex offset index

					/*auto offsetA = pos + EdgeVertexOffsets[edge][0];
					auto offsetB = pos + EdgeVertexOffsets[edge][1];

					auto sampleA = data[INDEX_3D(int(offsetA.x), int(offsetA.y), int(offsetA.z), count)] / 127.0f;
					auto sampleB = data[INDEX_3D(int(offsetB.x), int(offsetB.y), int(offsetB.z), count)] / 127.0f;

					auto vertexPosition = position + GetIntersection(offsetA, offsetB, sampleA, sampleB) * sizeMod;

					m_vertices.add(vertexPosition);
					m_indices.add(vertexIndex);

					vertexIndex++;*/
					edgeCase++;
				}
			}
		}
	}

	if (vertexIndex == 0)
		return;

	// TODO: update mesh
	
	// Calculate normals
	for (auto i = 0u; i < m_vertices.count(); i += 3)
	{
		auto p0 = m_vertices[i + 0];
		auto p1 = m_vertices[i + 1];
		auto p2 = m_vertices[i + 2];

		auto plane = Plane(p0, p1, p2);
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
