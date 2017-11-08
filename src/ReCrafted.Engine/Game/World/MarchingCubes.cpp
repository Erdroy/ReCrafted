// ReCrafted © 2016-2017 Always Too Late

#include "MarchingCubes.h"
#include "MCTables.h"
#include "Platform/Platform.h"
#include "Graphics/Rendering.h"

#define INDEX_3D(x, y, z, size) ((x) * (size) * (size) + (y) * (size) + (z))

float m_data[17][17][17];

float Sphere(Vector3 pos, float radius)
{
	const auto x = (16 - 2.0f) * 0.5f;

	auto origin = Vector3(x, x, x);
	return (pos - origin).length() - radius * radius;
}

float Sample(int x, int y, int z)
{
	if (y <= 4.0f)
		return -1;

	return 1;

	//return Sphere(Vector3(x, y, z), 5.0f);
}

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

void MarchingCubes::generate()
{
	auto start = Platform::getMiliseconds();

	Array<Vector3> vertices = {};
	Array<Vector3> normals = {};
	Array<Vector4> colors = {};
	Array<Vector2> uvs = {};
	Array<uint> indices = {};

	for(auto y = 0; y < 17; y ++)
	{
		for (auto x = 0; x < 17; x++)
		{
			for (auto z = 0; z < 17; z++)
			{
				m_data[x][y][z] = Sample(x, y, z);
			}
		}
	}

	auto vertexIndex = 0u;
	for (auto y = 0; y < 16; y++)
	{
		for (auto x = 0; x < 16; x++)
		{
			for (auto z = 0; z < 16; z++)
			{
				auto position = Vector3(float(x), float(y), float(z));

				byte corners = 0;
				for (auto i = 0; i < 8; i++)
				{
					if (m_data[x + CornerDeltasInt[i][0]][y + CornerDeltasInt[i][1]][z + CornerDeltasInt[i][2]] < 0)
						corners |= (1 << i);
				}

				if (corners == 0 || corners == 255)
					continue;

				auto edgeCase = 0;

				for(auto i = 0; i < 15; i ++)
				{
					auto edge = EdgesTable[corners][edgeCase];

					if (edge == -1)
						break;

					auto offsetA = position + EdgeVertexOffsets[edge][0];
					auto offsetB = position + EdgeVertexOffsets[edge][1];

					auto sampleA = m_data[int(offsetA.x)][int(offsetA.y)][int(offsetA.z)];
					auto sampleB = m_data[int(offsetB.x)][int(offsetB.y)][int(offsetB.z)];

					auto vertexPosition = GetIntersection(offsetA, offsetB, sampleA, sampleB);
					vertices.add(vertexPosition);
					normals.add(Vector3::up());
					uvs.add(Vector2::zero());
					colors.add(Vector4(1.0f, 0.0f, 1.0f, 1.0f));
					indices.add(vertexIndex);

					vertexIndex++;
					edgeCase++;
				}
			}
		}
	}

	auto end = Platform::getMiliseconds();
	auto diff = end - start;

	m_mesh = Mesh::createMesh();

	m_mesh->setVertices(vertices.data(), vertices.count());
	m_mesh->setNormals(normals.data());
	m_mesh->setUVs(uvs.data());
	m_mesh->setColors(colors.data());
	m_mesh->setIndices(indices.data(), indices.count());

	m_mesh->applyChanges();
	m_mesh->upload();

}

void MarchingCubes::draw()
{
	if (!m_mesh->isUploaded())
		return;

	Rendering::getInstance()->setState();

	auto matrix = Matrix::identity();
	Rendering::getInstance()->draw(m_mesh, &matrix);
}
