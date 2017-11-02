// ReCrafted © 2016-2017 Always Too Late

#include "VoxelMap.h"
#include "Graphics/Camera.h"
#include "Game/Universe.h"

void VoxelMap::MapRoot::addChunk(VoxelChunk* chunk, int x, int z)
{
	auto rsX = -(this->worldX - x * ChunkWidth) / ChunkWidth;
	auto rsZ = -(this->worldZ - z * ChunkWidth) / ChunkWidth; // TODO: cache this values in chunk class

	m_table[rsX * TableWidth + rsZ] = chunk;
}

void VoxelMap::MapRoot::removeChunk(int x, int z)
{
	auto rsX = -(this->worldX - x * ChunkWidth) / ChunkWidth;
	auto rsZ = -(this->worldZ - z * ChunkWidth) / ChunkWidth;

	auto chunk = m_table[rsX * TableWidth + rsZ];

	if(chunk)
	{
		chunk->dispose();
        m_vcm->m_chunks.remove(chunk);
	}

	m_table[rsX * TableWidth + rsZ] = nullptr;
}

VoxelChunk* VoxelMap::MapRoot::getChunk(int x, int z)
{
	auto rsX = -(this->worldX - x * ChunkWidth) / ChunkWidth;
	auto rsZ = -(this->worldZ - z * ChunkWidth) / ChunkWidth;

	return m_table[rsX * TableWidth + rsZ];
}

VoxelChunk* VoxelMap::MapRoot::getChunk(int index)
{
	return m_table[index];
}

VoxelMap::MapRoot* VoxelMap::findRoot(int x, int z)
{
	MapRoot* root = nullptr;
	auto found = false;

	auto worldX = x * ChunkWidth;
	auto worldZ = z * ChunkWidth;

	// iterate all of the map roots
	for (auto i = 0u; i < m_roots.size(); i ++)
	{
		// check if chunk is inside of current map root
		root = m_roots[i];

		if( root->worldX <= worldX && 
			root->worldZ <= worldZ && 
			root->worldX + MapRoot::TableWidthWS >= worldX + ChunkWidth &&
			root->worldZ + MapRoot::TableWidthWS >= worldZ + ChunkWidth)
		{
			found = true;
			break;
		}
	}

	return found ? root : nullptr;
}

void VoxelMap::addRoot(MapRoot* root)
{
	m_roots.add(root);

	root->m_vcm = this;

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

void VoxelMap::addChunk(VoxelChunk* chunk)
{
	auto root = findRoot(chunk->m_x, chunk->m_z);

	if(root == nullptr)
	{
		// make new root
		root = new MapRoot;

		// the chunk world pos
		root->worldX = chunkToRoot(chunk->m_x) * MapRoot::TableWidthWS;
		root->worldZ = chunkToRoot(chunk->m_z) * MapRoot::TableWidthWS;

		addRoot(root);
	}

	// set root ptr
	chunk->root = root;

	m_chunks.add(chunk);
	root->addChunk(chunk, chunk->m_x, chunk->m_z);
}

VoxelChunk* VoxelMap::findChunk(int x, int z)
{
	auto root = findRoot(x, z);

	if (root == nullptr)
		return nullptr;

	return root->getChunk(x, z);
}

Array<VoxelChunk*>* VoxelMap::getChunks()
{
	return &m_chunks;
}

void VoxelMap::draw()
{
	auto range = 500.0f;
	auto point = Camera::getMainCamera()->get_position();
	auto farChunkTop = int(point.z + range + 8) / ChunkWidth;
	auto farChunkBottom = int(point.z - range - 8) / ChunkWidth;
	auto farChunkLeft = int(point.x - range - 8) / ChunkWidth;
	auto farChunkRight = int(point.x + range + 8) / ChunkWidth;

	auto frustum = Camera::getMainCamera()->getBoundingFrustum();

	auto world = Universe::getCurrentWorld();

	// find far left-bottom root
	auto baseRoot = findRoot(farChunkLeft, farChunkBottom);

	if (baseRoot == nullptr)
	{
		world->generateChunk(farChunkLeft, farChunkBottom);
		baseRoot = findRoot(farChunkLeft, farChunkBottom);
	}

	auto drawnchunks = 0;

	VoxelChunk* chunk;
	auto root = baseRoot;
	for (auto x = farChunkLeft; x < farChunkRight; x++)
	{
		for (auto z = farChunkBottom; z < farChunkTop; z++)
		{
			// fast check if the chunk is in range
			auto rX = x*ChunkWidth - point.x;
			auto rZ = z*ChunkWidth - point.z;
			if (sqrt(rX * rX + rZ * rZ) > range)
				continue;

			// check if there is root
			if (!root)
			{
				// root not found, try to find
				root = findRoot(x, z);
				
				if(!root) // still there is no any root, return.
					return;
			}

			// check if currently selected root is the right root
			if(!root->contains(x, z))
			{
				// fast find root - traverse the neigh recursive tree

				// set base root as start
				root = baseRoot;

				// iterate from left up to right
				while (root && !root->containsX(x))
					root = root->m_neighRight;

				// iterate from bottom up to top
				while (root && !root->containsZ(z))
					root = root->m_neighTop;
			}

			// if root is not found, build new one
			if(!root)
			{
				// generate chunk on the new root which will be build
				world->generateChunk(x, z);
				continue;
			}

			// if the root is found
			chunk = root->getChunk(x, z);

			// if chunk exisits
			if(chunk)
			{
				if(chunk->m_mesh)
				{
					// frustum culling
					if (frustum.contains(chunk->getBoundingBox()))
					{
						// draw
						chunk->draw();
						drawnchunks++;
					}
				}
				else
				{
					if (chunk->m_processing || chunk->m_queued)
						continue;

					if (!chunk->m_mesh)
					{
						// queue chunk for meshing(voxel data will be ignored, because the chunk already has this)
						VoxelChunkProcessor::queue(chunk, VoxelChunkProcessor::QueueType::VoxelDataAndMesh);
					}
				}
			}
			else
			{
				// create new chunk and queue it to VCP
				world->generateChunk(x, z);
			}
		}
	}

	bgfx::dbgTextPrintf(1, 7, 0x4, "Drawn chunks: %d", drawnchunks);
}

void VoxelMap::dispose()
{
	// TODO: delete all map roots
}
