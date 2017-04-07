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

FORCEINLINE float calcAO(bool sideA, bool sideB, bool corner)
{
	if (sideA && sideB)
		return 1.0f;

	return (sideA ? 0.5f : 0) + (sideB ? 0.5f : 0) + (corner ? 0.5f : 0);
}

void VoxelChunk::generateVoxelData() // WARNING: this should be run in job queue!
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

void VoxelChunk::generateMesh(
	std::vector<Vector3>* verticesArray,
	std::vector<Vector3>* normalsArray,
	std::vector<Vector2>* uvsArray,
	std::vector<Vector4>* colorsArray,
	std::vector<uint>* indicesArray) // WARNING: this should be run in job queue!
{
	if (m_disposed)
		return;

	auto vertices_ptr = verticesArray;
	auto normals_ptr = normalsArray;
	auto uvs_ptr = uvsArray;
	auto colors_ptr = colorsArray;
	auto indices_ptr = indicesArray;

	const auto vec3_right = Vector3(1.0f, 0.0f, 0.0f);
	const auto vec3_up = Vector3(0.0f, 1.0f, 0.0f);
	const auto vec3_forward = Vector3(0.0f, 0.0f, 1.0f);

	const auto ambientStr = 0.45f;

	auto block_tf  = false, block_tb  = false, block_tl  = false, block_tr  = false;
	auto block_tfl = false, block_tfr = false, block_tbl = false, block_tbr = false;
	auto block_mfl = false, block_mfr = false, block_mbl = false, block_mbr = false;
	auto block_bf  = false, block_bb  = false, block_bl  = false, block_br  = false;
	auto block_bfl = false, block_bfr = false, block_bbl = false, block_bbr = false;

	float ao00, ao01, ao11, ao10;

	for (auto y = 0; y < ChunkHeight; y++)
	{
		for (auto x = 0; x < ChunkWidth; x++)
		{
			for (auto z = 0; z < ChunkWidth; z++)
			{
				if (m_voxels[y * (ChunkWidth * ChunkWidth) + z * ChunkWidth + x] == voxel_air)
					continue;

				auto origin = Vector3(float(x), float(y), float(z));

				auto leftFace = !getVoxelCC(x - 1, y, z);
				auto rightFace = !getVoxelCC(x + 1, y, z);
				auto bottomFace = !getVoxelCC(x, y - 1, z);
				auto upperFace = !getVoxelCC(x, y + 1, z);
				auto backFace = !getVoxelCC(x, y, z - 1);
				auto frontFace = !getVoxelCC(x, y, z + 1);

				if(leftFace || rightFace || bottomFace || upperFace || backFace || frontFace)
				{
					block_tf = getVoxelCC(x, y + 1, z + 1) != voxel_air;
					block_tb = getVoxelCC(x, y + 1, z - 1) != voxel_air;
					block_tl = getVoxelCC(x - 1, y + 1, z) != voxel_air;
					block_tr = getVoxelCC(x + 1, y + 1, z) != voxel_air;

					block_tfl = getVoxelCC(x - 1, y + 1, z + 1) != voxel_air;
					block_tfr = getVoxelCC(x + 1, y + 1, z + 1) != voxel_air;
					block_tbl = getVoxelCC(x - 1, y + 1, z - 1) != voxel_air;
					block_tbr = getVoxelCC(x + 1, y + 1, z - 1) != voxel_air;

					block_mfl = getVoxelCC(x - 1, y, z + 1) != voxel_air;
					block_mfr = getVoxelCC(x + 1, y, z + 1) != voxel_air;
					block_mbl = getVoxelCC(x - 1, y, z - 1) != voxel_air;
					block_mbr = getVoxelCC(x + 1, y, z - 1) != voxel_air;

					block_bf = getVoxelCC(x, y - 1, z + 1) != voxel_air;
					block_bb = getVoxelCC(x, y - 1, z - 1) != voxel_air;
					block_bl = getVoxelCC(x - 1, y - 1, z) != voxel_air;
					block_br = getVoxelCC(x + 1, y - 1, z) != voxel_air;

					block_bfl = getVoxelCC(x - 1, y - 1, z + 1) != voxel_air;
					block_bfr = getVoxelCC(x + 1, y - 1, z + 1) != voxel_air;
					block_bbl = getVoxelCC(x - 1, y - 1, z - 1) != voxel_air;
					block_bbr = getVoxelCC(x + 1, y - 1, z - 1) != voxel_air;
				}

				// left face
				if (leftFace)
				{
					ao00 = calcAO(block_mfl, block_bl, block_bfl);
					ao01 = calcAO(block_mfl, block_tl, block_tfl);
					ao11 = calcAO(block_mbl, block_tl, block_tbl);
					ao10 = calcAO(block_mbl, block_br, block_bbl);

					auto flippedQuad = ao01 + ao10 > ao00 + ao11;

					build_face(origin, vec3_up, vec3_forward, false, flippedQuad, vertices_ptr, normals_ptr, uvs_ptr, indices_ptr);

					colors_ptr->push_back({ ao10 * ambientStr, 0.0f, 0.0f, 0.0f });
					colors_ptr->push_back({ ao11 * ambientStr, 0.0f, 0.0f, 0.0f });
					colors_ptr->push_back({ ao01 * ambientStr, 0.0f, 0.0f, 0.0f });
					colors_ptr->push_back({ ao00 * ambientStr, 0.0f, 0.0f, 0.0f });
				}

				// right face
				if (rightFace)
				{
					ao00 = calcAO(block_br, block_mbr, block_bbr);
					ao01 = calcAO(block_tr, block_mbr, block_tbr);
					ao11 = calcAO(block_tr, block_mfr, block_tfr);
					ao10 = calcAO(block_br, block_mfr, block_bfr);

					auto flippedQuad = ao00 + ao11 > ao01 + ao10;

					build_face(origin + vec3_right, vec3_up, vec3_forward, true, flippedQuad, vertices_ptr, normals_ptr, uvs_ptr, indices_ptr);

					colors_ptr->push_back({ ao00 * ambientStr, 0.0f, 0.0f, 0.0f });
					colors_ptr->push_back({ ao01 * ambientStr, 0.0f, 0.0f, 0.0f });
					colors_ptr->push_back({ ao11 * ambientStr, 0.0f, 0.0f, 0.0f });
					colors_ptr->push_back({ ao10 * ambientStr, 0.0f, 0.0f, 0.0f });
				}

				// upper face
				if (upperFace)
				{
					ao00 = calcAO(block_tl, block_tb, block_tbl);
					ao01 = calcAO(block_tl, block_tf, block_tfl);
					ao11 = calcAO(block_tr, block_tf, block_tfr);
					ao10 = calcAO(block_tb, block_tr, block_tbr);

					auto flippedQuad = ao00 + ao11 > ao01 + ao10;

					build_face(origin + vec3_up, vec3_forward, vec3_right, true, flippedQuad, vertices_ptr, normals_ptr, uvs_ptr, indices_ptr);

					colors_ptr->push_back({ ao00 * ambientStr, 0.0f, 0.0f, 0.0f });
					colors_ptr->push_back({ ao01 * ambientStr, 0.0f, 0.0f, 0.0f });
					colors_ptr->push_back({ ao11 * ambientStr, 0.0f, 0.0f, 0.0f });
					colors_ptr->push_back({ ao10 * ambientStr, 0.0f, 0.0f, 0.0f });
				}

				// bottom face
				if (bottomFace && y - 1 >= 0)
				{
					ao00 = calcAO(block_bb, block_bl, block_bbl);
					ao01 = calcAO(block_bf, block_bl, block_bfl);
					ao11 = calcAO(block_bf, block_br, block_bfr);
					ao10 = calcAO(block_bb, block_br, block_bbr);

					auto flippedQuad = ao00 + ao11 > ao01 + ao10;

					build_face(origin, vec3_forward, vec3_right, false, flippedQuad, vertices_ptr, normals_ptr, uvs_ptr, indices_ptr);

					colors_ptr->push_back({ ao00 * ambientStr, 0.0f, 0.0f, 0.0f });
					colors_ptr->push_back({ ao01 * ambientStr, 0.0f, 0.0f, 0.0f });
					colors_ptr->push_back({ ao11 * ambientStr, 0.0f, 0.0f, 0.0f });
					colors_ptr->push_back({ ao10 * ambientStr, 0.0f, 0.0f, 0.0f });
				}

				// front face
				if (frontFace)
				{
					ao00 = calcAO(block_mfl, block_bf, block_bfl);
					ao01 = calcAO(block_mfl, block_tf, block_tfl);
					ao11 = calcAO(block_mfr, block_tf, block_tfr);
					ao10 = calcAO(block_mfr, block_bf, block_bfr);

					auto flippedQuad = ao00 + ao11 > ao01 + ao10;

					build_face(origin + vec3_forward, vec3_up, vec3_right, false, flippedQuad, vertices_ptr, normals_ptr, uvs_ptr, indices_ptr);

					colors_ptr->push_back({ ao00 * ambientStr, 0.0f, 0.0f, 0.0f });
					colors_ptr->push_back({ ao01 * ambientStr, 0.0f, 0.0f, 0.0f });
					colors_ptr->push_back({ ao11 * ambientStr, 0.0f, 0.0f, 0.0f });
					colors_ptr->push_back({ ao10 * ambientStr, 0.0f, 0.0f, 0.0f });
				}

				// back face
				if (backFace)
				{
					ao00 = calcAO(block_bb, block_mbl, block_bbl);
					ao01 = calcAO(block_mbl, block_tb, block_tbl);
					ao11 = calcAO(block_mbr, block_tb, block_tbr);
					ao10 = calcAO(block_mbr, block_bb, block_bbr);

					auto flippedQuad = ao00 + ao11 > ao01 + ao10;

					build_face(origin, vec3_up, vec3_right, true, flippedQuad, vertices_ptr, normals_ptr, uvs_ptr, indices_ptr);

					colors_ptr->push_back({ ao00 * ambientStr, 0.0f, 0.0f, 0.0f });
					colors_ptr->push_back({ ao01 * ambientStr, 0.0f, 0.0f, 0.0f });
					colors_ptr->push_back({ ao11 * ambientStr, 0.0f, 0.0f, 0.0f });
					colors_ptr->push_back({ ao10 * ambientStr, 0.0f, 0.0f, 0.0f });
				}
			}
		}
	}

	m_mesh = Mesh::createMesh();
	m_mesh->setVertices(vertices_ptr->data(), uint(vertices_ptr->size()));
	m_mesh->setUVs(uvs_ptr->data());
	m_mesh->setNormals(normals_ptr->data());
	m_mesh->setColors(colors_ptr->data());
	m_mesh->setIndices(indices_ptr->data(), uint(indices_ptr->size()));

	m_mesh->applyChanges();

	m_lastTimeVisible = Time::time();
}

void VoxelChunk::updateNeighs()
{
	if (!this->m_neighN)
	{
		// find chunk
		m_neighN = world->m_chunkMap->findChunk(m_x, m_z + 1);

		if (m_neighN)
			m_neighN->m_neighS = this;
	}

	if (!m_neighE)
	{
		// find chunk
		m_neighE = world->m_chunkMap->findChunk(m_x + 1, m_z);

		if (m_neighE)
			m_neighE->m_neighW = this;
	}

	if (!m_neighS)
	{
		// find chunk
		m_neighS = world->m_chunkMap->findChunk(m_x, m_z - 1);

		if (m_neighS)
			m_neighS->m_neighN = this;
	}

	if (!m_neighW)
	{
		// find chunk
		m_neighW = world->m_chunkMap->findChunk(m_x - 1, m_z);

		if (m_neighW)
			m_neighW->m_neighE = this;
	}

	// corners
	if (!m_neighNE)
	{
		// find chunk
		m_neighNE = world->m_chunkMap->findChunk(m_x + 1, m_z + 1);

		if (m_neighNE)
			m_neighNE->m_neighSW = this;
	}

	if (!m_neighSE)
	{
		// find chunk
		m_neighSE = world->m_chunkMap->findChunk(m_x + 1, m_z - 1);

		if (m_neighSE)
			m_neighSE->m_neighNW = this;
	}

	if (!m_neighSW)
	{
		// find chunk
		m_neighSW = world->m_chunkMap->findChunk(m_x - 1, m_z - 1);

		if (m_neighSW)
			m_neighSW->m_neighNE = this;
	}

	if (!m_neighNW)
	{
		// find chunk
		m_neighNW = world->m_chunkMap->findChunk(m_x - 1, m_z + 1);

		if (m_neighNW)
			m_neighNW->m_neighSE = this;
	}
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
			auto rootc = static_cast<VoxelMap::MapRoot*>(root);
			
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