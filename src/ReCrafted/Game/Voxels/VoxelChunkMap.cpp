// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "VoxelChunkMap.h"

void VoxelChunkMap::MapRoot::addChunk(VoxelChunk* chunk, int x, int z)
{
	auto rsX = -(this->worldX - x * ChunkWidth) / ChunkWidth;
	auto rsZ = -(this->worldZ - z * ChunkWidth) / ChunkWidth; // TODO: cache this values in chunk class

	m_table[rsX * TableWidth + rsZ] = chunk;
}

void VoxelChunkMap::MapRoot::removeChunk(int x, int z)
{
	auto rsX = -(this->worldX - x * ChunkWidth) / ChunkWidth;
	auto rsZ = -(this->worldZ - z * ChunkWidth) / ChunkWidth;

	m_table[rsX * TableWidth + rsZ] = nullptr;
}

VoxelChunk* VoxelChunkMap::MapRoot::getChunk(int x, int z)
{
	auto rsX = -(this->worldX - x * ChunkWidth) / ChunkWidth;
	auto rsZ = -(this->worldZ - z * ChunkWidth) / ChunkWidth;

	return m_table[rsX * TableWidth + rsZ];
}

VoxelChunkMap::MapRoot* VoxelChunkMap::findRoot(int x, int z)
{
	MapRoot* root = nullptr;
	auto found = false;

	auto worldSize = MapRoot::TableWidth * ChunkWidth;

	auto worldX = x * ChunkWidth;
	auto worldZ = z * ChunkWidth;

	// iterate all of the map roots
	for (auto i = 0u; i < m_roots.size(); i ++)
	{
		// check if chunk is inside of current map root
		root = m_roots[i];

		if( root->worldX <= worldX && 
			root->worldZ <= worldZ && 
			root->worldX + worldSize >= worldX + ChunkWidth &&
			root->worldZ + worldSize >= worldZ + ChunkWidth)
		{
			found = true;
			break;
		}
	}

	return found ? root : nullptr;
}

void VoxelChunkMap::addChunk(VoxelChunk* chunk)
{
	auto root = findRoot(chunk->m_x, chunk->m_z);

	if(root == nullptr)
	{
		// make new root
		root = new MapRoot;

		// the chunk world pos
		auto worldX = chunk->m_x * ChunkWidth;
		auto worldZ = chunk->m_z * ChunkWidth;

		root->worldX = Math::roundDown(worldX, 1024);
		root->worldZ = Math::roundDown(worldZ, 1024);

		m_roots.push_back(root);
	}

	m_chunks.push_back(chunk);
	root->addChunk(chunk, chunk->m_x, chunk->m_z);
}

VoxelChunk* VoxelChunkMap::findChunk(int x, int z)
{
	auto root = findRoot(x, z);

	if (root == nullptr)
		return nullptr;

	return root->getChunk(x, z);
}

std::vector<VoxelChunk*>* VoxelChunkMap::getChunks()
{
	return &m_chunks;
}

void VoxelChunkMap::getVisibleChunks(Vector2 pointXZ, float range, std::vector<VoxelChunk*>* chunks)
{

}

void VoxelChunkMap::dispose()
{
	// TODO: delete all map roots
}
