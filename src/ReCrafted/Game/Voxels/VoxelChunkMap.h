// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef VOXELCHUNKMAP_H
#define VOXELCHUNKMAP_H

// includes
#include "VoxelChunk.h"

class VoxelChunkMap
{
	class MapRoot
	{
	public:
		static const int TableWidth = 64; // 64(chunk count)
		static const int TableWidthWS = 64 * ChunkWidth; // world: 64(chunk count) * 16(chunk size)

	public:
		int worldX = 0;
		int worldZ = 0;

		void addChunk(VoxelChunk* chunk, int x, int z);
		void removeChunk(int x, int z);
		VoxelChunk* getChunk(int x, int z);
		VoxelChunk* getChunk(int index);

	private:
		VoxelChunk* m_table[TableWidth * TableWidth] = {};
	};

private:
	std::vector<MapRoot*> m_roots = {};
	std::vector<VoxelChunk*> m_chunks = {};

private:
	MapRoot* findRoot(int x, int z);

public:
	void addChunk(VoxelChunk* chunk);
	VoxelChunk* findChunk(int x, int z);
	std::vector<VoxelChunk*>* getChunks();
	void getVisibleChunks(Vector2 point, float range, std::vector<VoxelChunk*>* chunks);

	void dispose();
};

#endif // VOXELCHUNKMAP_H
