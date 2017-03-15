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

VoxelChunk* VoxelChunkMap::MapRoot::getChunk(int index)
{
	return m_table[index];
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

void VoxelChunkMap::getVisibleChunks(Vector2 point, float range, std::vector<VoxelChunk*>* chunks)
{
	auto sqrDistance = range * range;

	MapRoot* root = nullptr;
	VoxelChunk* chunk = nullptr;
	for (auto i = 0u; i < m_roots.size(); i++) 
	{
		root = m_roots[i];

		auto wsX = float(root->worldX);
		auto wsZ = float(root->worldZ);

		auto deltaX = point.X - Math::maxf(wsX, Math::minf(point.X, wsX + MapRoot::TableWidthWS));
		auto deltaY = point.Y - Math::maxf(wsZ, Math::minf(point.Y, wsZ + MapRoot::TableWidthWS));

		auto isinrange = deltaX * deltaX + deltaY * deltaY < sqrDistance;
		auto isinside = point.X > wsX && point.X < wsX + MapRoot::TableWidthWS && point.Y > wsZ && point.Y < wsZ + MapRoot::TableWidthWS;

		if (isinrange || isinside)
		{
			for (auto j = 0; j < MapRoot::TableWidth * MapRoot::TableWidth; j++) // TODO: optimize this loop, skip x axes that are out of range
			{
				chunk = root->getChunk(j);

				if (!chunk)
					continue;

				wsX = float(chunk->m_x * ChunkWidth);
				wsZ = float(chunk->m_z * ChunkWidth);

				deltaX = point.X - Math::maxf(wsX, Math::minf(point.X, wsX + ChunkWidth));
				deltaY = point.Y - Math::maxf(wsZ, Math::minf(point.X, wsZ + ChunkWidth));

				if (deltaX * deltaX + deltaY * deltaY < sqrDistance)
					chunks->push_back(chunk);
			}
		}
	}
}

void VoxelChunkMap::dispose()
{
	// TODO: delete all map roots
}
