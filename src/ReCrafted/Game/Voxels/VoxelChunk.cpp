// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "VoxelChunk.h"
#include "../../Core/Profiler.h"
#include <vector>

FORCEINLINE void build_face(
	Vector3 origin,
	Vector3 up,
	Vector3 right,
	bool reversed,
	bool inverted,
	std::vector<Vector3>* vertices,
	std::vector<Vector3>* normals,
	std::vector<Vector2>* uvs,
	std::vector<uint>* indices
	)
{
	auto index = uint(vertices->size());

	auto a = origin;
	auto b = origin + up;
	auto c = origin + up + right;
	auto d = origin + right;

	auto normal = Vector3::normalize(Vector3::cross(b - a, c - a));

	vertices->push_back(a);
	vertices->push_back(b);
	vertices->push_back(c);
	vertices->push_back(d);

	normals->push_back(normal);
	normals->push_back(normal);
	normals->push_back(normal);
	normals->push_back(normal);

	uvs->push_back(Vector2(0.0f, 0.0f));
	uvs->push_back(Vector2(0.0f, 1.0f));
	uvs->push_back(Vector2(1.0f, 1.0f));
	uvs->push_back(Vector2(1.0f, 0.0f));

	if (!reversed)
	{
		if (inverted)
		{
			indices->push_back(index + 1u);
			indices->push_back(index + 2u);
			indices->push_back(index + 3u);
			indices->push_back(index + 3u);
			indices->push_back(index + 0u);
			indices->push_back(index + 1u);
		}
		else
		{
			indices->push_back(index + 0u);
			indices->push_back(index + 1u);
			indices->push_back(index + 2u);
			indices->push_back(index + 2u);
			indices->push_back(index + 3u);
			indices->push_back(index + 0u);
		}
	}
	else
	{
		if (inverted)
		{
			indices->push_back(index + 2u);
			indices->push_back(index + 1u);
			indices->push_back(index + 3u);
			indices->push_back(index + 0u);
			indices->push_back(index + 3u);
			indices->push_back(index + 1u);
		}
		else
		{
			indices->push_back(index + 1u);
			indices->push_back(index + 0u);
			indices->push_back(index + 2u);
			indices->push_back(index + 3u);
			indices->push_back(index + 2u);
			indices->push_back(index + 0u);
		}
	}
}

void VoxelChunk::worker_dataGenerate()
{
	Profiler::beginProfile();
	for(auto y = 0; y < ChunkHeight; y ++)
	{
		for(auto x = 0; x < ChunkWidth; x ++)
		{
			for (auto z = 0; z < ChunkWidth; z++)
			{
				if(y == 0)
					m_voxels[x][y][z] = 1u;
				else
					m_voxels[x][y][z] = voxel_air;
			}
		}
	}
	Profiler::endProfile("Chunk voxel data generated in %0.7f ms.");
}

void VoxelChunk::worker_meshGenerate()
{
	Profiler::beginProfile();
	{
		m_mesh = Mesh::createMesh();

		std::vector<Vector3> vertices = {};
		std::vector<Vector3> normals = {};
		std::vector<Vector2> uvs = {};
		std::vector<uint> indices = {};

		auto vertices_ptr = &vertices;
		auto normals_ptr = &normals;
		auto uvs_ptr = &uvs;
		auto indices_ptr = &indices;

		//std::vector<Vector4> colors = {};

		const auto vec3_right = Vector3(1.0f, 0.0f, 0.0f);
		const auto vec3_up = Vector3(0.0f, 1.0f, 0.0f);
		const auto vec3_forward = Vector3(0.0f, 0.0f, 1.0f);

		Profiler::beginProfile();
		for (auto y = 0; y < ChunkHeight; y++)
		{
			for (auto x = 0; x < ChunkWidth; x++)
			{
				for (auto z = 0; z < ChunkWidth; z++)
				{
					if (m_voxels[x][y][z] == voxel_air)
						continue;

					auto origin = Vector3(float(x), float(y), float(z));

					// left face
					build_face(origin, vec3_up, vec3_forward, false, false, vertices_ptr, normals_ptr, uvs_ptr, indices_ptr);

					// right face
					build_face(origin + vec3_right, vec3_up, vec3_forward, true, false, vertices_ptr, normals_ptr, uvs_ptr, indices_ptr);

					// upper face
					build_face(origin + vec3_up, vec3_forward, vec3_right, true, false, vertices_ptr, normals_ptr, uvs_ptr, indices_ptr);
					
					// bottom face
					build_face(origin, vec3_forward, vec3_right, false, false, vertices_ptr, normals_ptr, uvs_ptr, indices_ptr);

					// front face
					build_face(origin + vec3_forward, vec3_up, vec3_right, false, false, vertices_ptr, normals_ptr, uvs_ptr, indices_ptr);

					// back face
					build_face(origin, vec3_up, vec3_right, true, false, vertices_ptr, normals_ptr, uvs_ptr, indices_ptr);
				}
			}
		}
		Profiler::endProfile("Mesh build took %0.7f ms.");

		m_mesh->setVertices(vertices.data(), uint(vertices.size()));
		m_mesh->setUVs(uvs.data());
		m_mesh->setNormals(normals.data());
		m_mesh->setIndices(indices.data(), uint(indices.size()));

		// upload now for a test
		meshUpload();
	}
	Profiler::endProfile("Chunk mesh generated in %0.7f ms.");
}

void VoxelChunk::update()
{
}

void VoxelChunk::simulate()
{
}
