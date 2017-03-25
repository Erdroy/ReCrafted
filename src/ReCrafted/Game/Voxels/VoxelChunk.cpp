// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "VoxelChunk.h"
#include "../../Core/Profiler.h"
#include "Generator/VoxelGenerator.h"
#include "VoxelWorld.h"
#include <vector>
#include "../../Graphics/Camera.h"

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
	// TODO: this code should be higly optimized.

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
	// create voxels
	m_voxels = new voxelid[ChunkWidth * ChunkHeight * ChunkWidth];

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

	m_lastTimeVisible = Time::time();
}

void VoxelChunk::worker_meshGenerate() // WARNING: this should be run in job queue!
{
	std::vector<Vector3> vertices = {};
	std::vector<Vector3> normals = {};
	std::vector<Vector2> uvs = {};
	//std::vector<Vector4> colors = {};
	std::vector<uint> indices = {};

	{
		auto vertices_ptr = &vertices;
		auto normals_ptr = &normals;
		auto uvs_ptr = &uvs;
		auto indices_ptr = &indices;

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
					if (!getVoxelCC(x, y - 1, z) && y - 1 >= 0)
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

		m_mesh = Mesh::createMesh();
		m_mesh->setVertices(vertices.data(), uint(vertices.size()));
		m_mesh->setUVs(uvs.data());
		m_mesh->setNormals(normals.data());
		m_mesh->setIndices(indices.data(), uint(indices.size()));

		m_mesh->applyChanges();

		m_lastTimeVisible = Time::time();
	}
}

void VoxelChunk::updateNeighs()
{
	world->findNeighs(this);
}

void VoxelChunk::update()
{
	if (m_processing || m_queued)
		return;

	auto timeNotUsed = Time::time() - m_lastTimeVisible;

	auto chunkPos = Vector2(float(m_x) * ChunkWidth + ChunkWidth / 2, float(m_z) * ChunkWidth + ChunkWidth / 2);
	auto camPos = Camera::getMainCamera()->get_position();
	auto distance = Vector2::distance(chunkPos, Vector2(camPos.x, camPos.z));

	if (distance <= 564.0f) // TODO: gameoptions range
		return;

	if (m_mesh) 
	{
		if (timeNotUsed > 5.0f) // unload chunk data when out of view for half a sec or grater and is out of view range about 64(4 chunks) units/meters
		{
			// unload mesh
			m_mesh->dispose();
			m_mesh = nullptr;
		}
		return;
	}

	// unload if chunk is more than visibility range(+128 units, 8 chunks) meters away
	if (distance >= 564.0f + 64.0f) // TODO: gameoptions range
	{
		if (timeNotUsed > 10.0f)
		{
			// TODO: queue to unload
			auto rootc = static_cast<VoxelChunkMap::MapRoot*>(root);
			
			if(rootc)
				rootc->removeChunk(m_x, m_z);
			else
				dispose();
		}
	}
}

void VoxelChunk::simulate()
{
	if (m_processing)
		return;

}