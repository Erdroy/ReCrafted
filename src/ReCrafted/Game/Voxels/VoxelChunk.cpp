// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "VoxelChunk.h"
#include "../../Core/Profiler.h"
#include "Generator/VoxelGenerator.h"
#include "VoxelWorld.h"
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

	if (reversed)
		normal = Vector3() - normal;

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

void VoxelChunk::worker_dataGenerate() // WARNING: this should be run in job queue!
{
	VoxelGenerator::beginChunk(m_x * ChunkWidth, m_z * ChunkWidth);
	for(auto y = 0; y < ChunkHeight; y ++)
	{
		for (auto x = 0; x < ChunkWidth; x++)
		{
			for (auto z = 0; z < ChunkWidth; z++)
			{
				m_voxels[y * (ChunkWidth * ChunkWidth) + z * ChunkWidth + x] = VoxelGenerator::generate(x, y, z);
			}
		}
	}
	VoxelGenerator::endChunk();
}

void VoxelChunk::worker_meshGenerate() // WARNING: this should be run in job queue!
{
	std::vector<Vector3> vertices = {};
	std::vector<Vector3> normals = {};
	std::vector<Vector2> uvs = {};
	std::vector<uint> indices = {};

	{
		// find neighs
		world->findNeighs(this);

		// generate neigh chunks if not generated already
		world->initializeNeighs(this);

		m_mesh = Mesh::createMesh();

		auto vertices_ptr = &vertices;
		auto normals_ptr = &normals;
		auto uvs_ptr = &uvs;
		auto indices_ptr = &indices;

		//std::vector<Vector4> colors = {};

		const auto vec3_right = Vector3(1.0f, 0.0f, 0.0f);
		const auto vec3_up = Vector3(0.0f, 1.0f, 0.0f);
		const auto vec3_forward = Vector3(0.0f, 0.0f, 1.0f);

		for (auto y = 0; y < ChunkHeight; y++)
		{
			for (auto x = 0; x < ChunkWidth; x++)
			{
				for (auto z = 0; z < ChunkWidth; z++)
				{
					if (m_voxels[y * (ChunkWidth * ChunkWidth) + z * ChunkWidth + x] == voxel_air)
						continue;

					auto origin = Vector3(float(x), float(y), float(z));

					// left face
					if(!getVoxelCC(x - 1, y, z))
						build_face(origin, vec3_up, vec3_forward, false, false, vertices_ptr, normals_ptr, uvs_ptr, indices_ptr);

					// right face
					if (!getVoxelCC(x + 1, y, z))
						build_face(origin + vec3_right, vec3_up, vec3_forward, true, false, vertices_ptr, normals_ptr, uvs_ptr, indices_ptr);

					// upper face
					if (!getVoxelCC(x, y + 1, z))
						build_face(origin + vec3_up, vec3_forward, vec3_right, true, false, vertices_ptr, normals_ptr, uvs_ptr, indices_ptr);
					
					// bottom face
					if (!getVoxelCC(x, y - 1, z) && y - 1 >= 9)
						build_face(origin, vec3_forward, vec3_right, false, false, vertices_ptr, normals_ptr, uvs_ptr, indices_ptr);

					// front face
					if (!getVoxelCC(x, y, z + 1))
						build_face(origin + vec3_forward, vec3_up, vec3_right, false, false, vertices_ptr, normals_ptr, uvs_ptr, indices_ptr);

					// back face
					if (!getVoxelCC(x, y, z - 1))
						build_face(origin, vec3_up, vec3_right, true, false, vertices_ptr, normals_ptr, uvs_ptr, indices_ptr);
				}
			}
		}

		m_mesh->setVertices(vertices.data(), uint(vertices.size()));
		m_mesh->setUVs(uvs.data());
		m_mesh->setNormals(normals.data());
		m_mesh->setIndices(indices.data(), uint(indices.size()));

		// upload now for a test
		meshUpload();
	}
}

void VoxelChunk::update()
{
}

void VoxelChunk::simulate()
{
}