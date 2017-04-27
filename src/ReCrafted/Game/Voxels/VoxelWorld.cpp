// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "VoxelWorld.h"
#include "Generator/VoxelGenerator.h"
#include "VoxelChunkProcessor.h"
#include "../../Graphics/Camera.h"
#include "../../Core/Input.h"
#include <string>

int VoxelWorld::m_availableUploads;

void VoxelWorld::update_digplace()
{
	auto dig = Input::isKeyDown(Key_Mouse0);
	auto place = Input::isKeyDown(Key_Mouse1);
	auto nuke = Input::isKeyDown(Key_Mouse2);

	if(Input::isKey(Key_Space))
		nuke = Input::isKey(Key_Mouse2);

	if(nuke)
	{
		RaycastHit hit;
		if (raycast(Camera::getMainCamera()->get_position(), Camera::getMainCamera()->get_forward(), 64.0f, &hit))
		{
			auto const radius = 32;

			auto chunk = getVoxelChunk(hit.position + hit.normal);
			auto blockpos = hit.position + hit.normal;

			auto xbase = chunk->toVoxelSpaceX(blockpos.x);
			auto ybase = int(floor(blockpos.y));
			auto zbase = chunk->toVoxelSpaceZ(blockpos.z);

			for (auto y = ybase + -radius; y < ybase + radius; y++)
			{
				for (auto x = xbase + -radius; x < xbase + radius; x++)
				{
					for (auto z = zbase + -radius; z < zbase + radius; z++)
					{
						if (Vector3::distance(
							Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)),
							Vector3(static_cast<float>(xbase), static_cast<float>(ybase), static_cast<float>(zbase))) > sqrt(float(radius)))
							continue;

						VoxelChunk* nchunk;
						if(Input::isKey(Key_Mouse1))
							nchunk = chunk->setVoxelCC(1, x, y, z);
						else
							nchunk = chunk->setVoxelCC(0, x, y, z);

						// check if the block was on the edge, if so, also rebuild the needed neighs
						if (VoxelChunk::isOnEdge(x) || VoxelChunk::isOnEdge(z))
						{
							VoxelChunk* neigh_a = nullptr;
							VoxelChunk* neigh_b = nullptr;
							VoxelChunk* neigh_c = nullptr;

							chunk->getEdgeNeighs(x, z, &neigh_a, &neigh_b, &neigh_c);

							if (neigh_a)
								neigh_a->updateMesh();

							if (neigh_b)
								neigh_b->updateMesh();

							if (neigh_c)
								neigh_c->updateMesh();
						}

						if(nchunk)
							nchunk->updateMesh();
					}
				}
			}
		}
	}

	if(place)
	{
		// cast ray
		RaycastHit hit;
		if (raycast(Camera::getMainCamera()->get_position(), Camera::getMainCamera()->get_forward(), 7.0f, &hit))
		{
			// hit!
			auto chunk = getVoxelChunk(hit.position + hit.normal);

			auto blockpos = hit.position + hit.normal;

			auto x = chunk->toVoxelSpaceX(blockpos.x);
			auto y = int(floor(blockpos.y));
			auto z = chunk->toVoxelSpaceZ(blockpos.z);

			chunk->setVoxel(1, x, y, z);

			// rebuild chunk
			chunk->updateMesh();

			// check if the block was on the edge, if so, also rebuild the needed neighs
			if (VoxelChunk::isOnEdge(x) || VoxelChunk::isOnEdge(z))
			{
				VoxelChunk* neigh_a = nullptr;
				VoxelChunk* neigh_b = nullptr;
				VoxelChunk* neigh_c = nullptr;

				chunk->getEdgeNeighs(x, z, &neigh_a, &neigh_b, &neigh_c);

				if (neigh_a)
					neigh_a->updateMesh();

				if (neigh_b)
					neigh_b->updateMesh();

				if (neigh_c)
					neigh_c->updateMesh();
			}
		}
	}

	if(dig)
	{
		// cast ray
		RaycastHit hit;
		if(raycast(Camera::getMainCamera()->get_position(), Camera::getMainCamera()->get_forward(), 7.0f, &hit))
		{
			// hit!
			auto chunk = getVoxelChunk(hit.position);

			auto x = chunk->toVoxelSpaceX(hit.position.x);
			auto y = int(floor(hit.position.y));
			auto z = chunk->toVoxelSpaceZ(hit.position.z);

			chunk->setVoxel(0, x, y, z);

			// rebuild chunk
			chunk->updateMesh();

			// check if the block was on the edge, if so, also rebuild the needed neighs
			if(VoxelChunk::isOnEdge(x) || VoxelChunk::isOnEdge(z))
			{
				VoxelChunk* neigh_a = nullptr;
				VoxelChunk* neigh_b = nullptr;
				VoxelChunk* neigh_c = nullptr;

				chunk->getEdgeNeighs(x, z, &neigh_a, &neigh_b, &neigh_c);
				
				if(neigh_a)
					neigh_a->updateMesh();

				if (neigh_b)
					neigh_b->updateMesh();

				if (neigh_c)
					neigh_c->updateMesh();
			}
		}
	}
}

void VoxelWorld::init()
{
	// initialize generator
	VoxelGenerator::initialize();

	// create voxel grid
	m_chunkMap = new VoxelMap;
}

void VoxelWorld::update()
{
	for(auto chunk : *m_chunkMap->getChunks())
	{
		chunk->update();
	}

	// update block-place/dig
	update_digplace();
}

void VoxelWorld::simulate()
{
	for (auto chunk : *m_chunkMap->getChunks())
	{
		// TODO: check if chunk should be simulated
		chunk->simulate();
	}
}

void VoxelWorld::draw()
{
	m_availableUploads = 16; // 16 mesh uploads per frame

	m_chunkMap->draw();
}

VoxelChunk* VoxelWorld::generateChunk(int x, int z)
{
	auto chunk = new VoxelChunk;
	chunk->world = this;
	chunk->m_x = x;
	chunk->m_z = z;

	// add to chunk map
	m_chunkMap->addChunk(chunk);

	// queue chunk for generation
	VoxelChunkProcessor::queue(chunk, VoxelChunkProcessor::QueueType::VoxelData);

	return chunk;
}

VoxelChunk* VoxelWorld::getVoxelChunk(Vector3 containedPoint) const
{
	return m_chunkMap->findChunk(VoxelChunk::toChunkSpaceX(containedPoint.x), VoxelChunk::toChunkSpaceZ(containedPoint.z));
}

bool VoxelWorld::raycast(Vector3 origin, Vector3 direction, float length, RaycastHit* hit)
{
	auto baseChunk = getVoxelChunk(origin);

	if (baseChunk == nullptr)
		return false;

	if (baseChunk->raycast(origin, direction, length, hit))
		return true;

	return false;
}
