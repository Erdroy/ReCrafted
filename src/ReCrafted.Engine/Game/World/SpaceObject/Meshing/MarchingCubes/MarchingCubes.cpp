// ReCrafted © 2016-2017 Always Too Late

#include "MarchingCubes.h"
#include "MCTables.h"
#include "Platform/Platform.h"
#include "Graphics/Rendering.h"
#include "Core/Math/Plane.h"
#include "Core/Math/FastNoiseSIMD/FastNoiseSIMD.h"

#define INDEX_3D(x, y, z, size) ((x) * (size) * (size) + (y) * (size) + (z))

#define DATA_SIZE 129

float m_data[DATA_SIZE][DATA_SIZE][DATA_SIZE];
FastNoiseSIMD* noise_terrain = nullptr;
float* noise_terrain_set = nullptr;

float Sphere(Vector3 pos, float radius)
{
	const auto x = 16 * 0.5f;

	auto origin = Vector3(x, x, x);
	return (pos - origin).length() - radius * radius;
} 

float SimplexNoise(float _x, float _y, float _z)
{
	auto x = static_cast<int>(_x);
	auto y = static_cast<int>(_y);
	auto z = static_cast<int>(_z);
	
	return noise_terrain_set[INDEX_3D(x, y, z, DATA_SIZE)];
}

float Noise(Vector3 pos)
{
	return SimplexNoise(pos.x, pos.y, pos.z);
}

float Terrain(const Vector3& position)
{
	return (position.y - 8.0f) - Noise(position) * 4.0f + SimplexNoise(position.x, 8.0f, position.y);
}

float Sample(int x, int y, int z)
{
	auto pos = Vector3(float(x), float(y), float(z));
	return Terrain(pos);
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

Vector3 GetNormal(Vector3 position)
{
	auto x = int(position.x);
	auto y = int(position.y);
	auto z = int(position.z);

	auto h = 1;
	auto dxp = m_data[x + h][y][z];
	auto dxm = m_data[x - h][y][z];
	auto dyp = m_data[x][y + h][z];
	auto dym = m_data[x][y - h][z];
	auto dzp = m_data[x][y][z + h];
	auto dzm = m_data[x][y][z - h];

	auto gradient = Vector3(dxp - dxm, dyp - dym, dzp - dzm);
	gradient.normalize();
	return gradient;
}

void MarchingCubes::generate()
{
	auto start = Platform::getMiliseconds();

	FastNoiseSIMD::SetSIMDLevel(-1);

	noise_terrain = FastNoiseSIMD::NewFastNoiseSIMD(100);
	noise_terrain->SetNoiseType(FastNoiseSIMD::Simplex);
	noise_terrain->SetFrequency(0.06f);
	noise_terrain->SetFractalType(FastNoiseSIMD::FBM);
	noise_terrain->SetFractalOctaves(4);
	noise_terrain->SetFractalLacunarity(2.0f);
	noise_terrain->SetFractalGain(0.5f);

	noise_terrain_set = noise_terrain->GetSimplexFractalSet(0, 0, 0, DATA_SIZE, DATA_SIZE, DATA_SIZE);

	Array<Vector3> vertices = {};
	Array<uint> indices = {};

	for (auto y = 0; y < DATA_SIZE; y++)
	{
		for (auto x = 0; x < DATA_SIZE; x++)
		{
			for (auto z = 0; z < DATA_SIZE; z++)
			{
				m_data[x][y][z] = Sample(x, y, z);
			}
		}
	}

	auto vertexIndex = 0u;
	for (auto y = 0; y < DATA_SIZE - 1; y++)
	{
		for (auto x = 0; x < DATA_SIZE - 1; x++)
		{
			for (auto z = 0; z < DATA_SIZE - 1; z++)
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

				for (auto i = 0; i < 15; i++)
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
					indices.add(vertexIndex);

					vertexIndex++;
					edgeCase++;
				}
			}
		}
	}

	auto normals = Array<Vector3>(vertices.count());
	auto colors = Array<Vector4>(vertices.count());
	auto uvs = Array<Vector2>(vertices.count());

	// Calculate normals
	for (auto i = 0u; i < vertices.count(); i += 3)
	{
		auto p0 = vertices[i + 0];
		auto p1 = vertices[i + 1];
		auto p2 = vertices[i + 2];

		auto plane = Plane(p0, p1, p2);
		plane.normalize();

		normals.add(plane.normal);
		normals.add(plane.normal);
		normals.add(plane.normal);

		uvs.add(Vector2::zero());
		uvs.add(Vector2::zero());
		uvs.add(Vector2::zero());

		colors.add(Vector4(85 / 255.0f, 60 / 255.0f, 50 / 255.0f, 1.0f));
		colors.add(Vector4(85 / 255.0f, 60 / 255.0f, 50 / 255.0f, 1.0f));
		colors.add(Vector4(85 / 255.0f, 60 / 255.0f, 50 / 255.0f, 1.0f));

	}

	auto end = Platform::getMiliseconds();
	auto diff = end - start;

	if (vertices.count() == 0)
		return;

	if (diff < 0) // hack, do not remove diff in release build
		throw;

	m_mesh = Mesh::createMesh();

	m_mesh->setVertices(vertices.data(), vertices.count());
	m_mesh->setUVs(uvs.data());
	m_mesh->setNormals(normals.data());
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
