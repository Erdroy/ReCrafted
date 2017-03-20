// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "VoxelChunkMap.h"
#include "../../Graphics/Camera.h"

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

void VoxelChunkMap::addRoot(MapRoot* root)
{
	m_roots.push_back(root);

	auto x = root->worldX / ChunkWidth;
	auto z = root->worldZ / ChunkWidth;

	auto farTop = (root->worldZ + MapRoot::TableWidthWS) / ChunkWidth;
	auto farBottom = root->worldZ / ChunkWidth;
	auto farLeft = root->worldX / ChunkWidth;
	auto farRight = (root->worldX + MapRoot::TableWidthWS) / ChunkWidth;

	// update neighs
	// find all neighs and update given root
	root->m_neighTop = findRoot(x, farTop + 1);
	root->m_neighBottom = findRoot(x, farBottom - 1);
	root->m_neighLeft = findRoot(farLeft - 1, z);
	root->m_neighRight = findRoot(farRight + 1, z);

	// update all neighs setting given root as correct neigh
	if (root->m_neighTop)
		root->m_neighTop->m_neighBottom = root;

	if (root->m_neighBottom)
		root->m_neighBottom->m_neighTop = root;

	if (root->m_neighLeft)
		root->m_neighLeft->m_neighRight = root;

	if (root->m_neighRight)
		root->m_neighRight->m_neighLeft = root;
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

		addRoot(root);
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

void VoxelChunkMap::draw()
{
	auto range = 250.0f;
	auto chunkSpaceRange = range / ChunkWidth;
	auto sqrCDistance = chunkSpaceRange * chunkSpaceRange;
	auto point = Camera::getMainCamera()->getPosition();
	auto farChunkTop = int(point.Z + range + 8) / ChunkWidth;
	auto farChunkBottom = int(point.Z - range - 8) / ChunkWidth;
	auto farChunkLeft = int(point.X - range - 8) / ChunkWidth;
	auto farChunkRight = int(point.X + range + 8) / ChunkWidth;

	// find far left-bottom root
	auto baseRoot = findRoot(farChunkLeft, farChunkBottom);

	for (auto x = farChunkLeft; x < farChunkRight; x++)
	{
		for (auto z = farChunkBottom; z < farChunkTop; z++)
		{
			// fast check if the chunk is in range
			if (x * x + z * z > sqrCDistance)
				continue;

			// TODO: fast find root - traverse the neigh recursive tree?
			// if root is not found, build new one
			//	then create new chunk and queue it to VCP
			//  EXIT
			// if the root is found
			//  if chunk exisits
			//   draw
			//   EXIT
			//  else
			//   create new chunk and queue it to VCP
			//   EXIT
			
			// TEMPORARY: for debugging
			auto chunk = findChunk(x, z);

			if (chunk)
				chunk->draw();
		}
	}

	// old method:
	/*MapRoot* root;
	VoxelChunk* chunk;
	for (auto i = 0u; i < m_roots.size(); i++)
	{
		root = m_roots[i];

		auto wsX = float(root->worldX);
		auto wsZ = float(root->worldZ);

		auto deltaX = point.X - Math::maxf(wsX, Math::minf(point.X, wsX + MapRoot::TableWidthWS));
		auto deltaY = point.Z - Math::maxf(wsZ, Math::minf(point.Z, wsZ + MapRoot::TableWidthWS));

		auto isinrange = deltaX * deltaX + deltaY * deltaY < sqrDistance;
		auto isinside = point.X > wsX && point.X < wsX + MapRoot::TableWidthWS && point.Z > wsZ && point.Z < wsZ + MapRoot::TableWidthWS;

		if (isinrange || isinside)
		{
			for (auto j = 0; j < MapRoot::TableWidth * MapRoot::TableWidth; j++) // TODO: optimize this loop, skip x and z axes that are out of range
			{
				chunk = root->getChunk(j);

				if (!chunk)
					continue;

				wsX = float(chunk->m_x * ChunkWidth);
				wsZ = float(chunk->m_z * ChunkWidth);

				deltaX = point.X - Math::maxf(wsX, Math::minf(point.X, wsX + ChunkWidth));
				deltaY = point.Z - Math::maxf(wsZ, Math::minf(point.Z, wsZ + ChunkWidth));

				if (deltaX * deltaX + deltaY * deltaY < sqrDistance)
				{
					chunk->draw();
				}
			}
		}
	}*/
}

void VoxelChunkMap::dispose()
{
	// TODO: delete all map roots
}
