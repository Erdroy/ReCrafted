// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef VOXELCHUNKMAP_H
#define VOXELCHUNKMAP_H

// includes
#include "VoxelChunk.h"

/// <summary>
/// VoxelChunkMap class.
/// </summary>
class VoxelChunkMap
{
	class MapRoot
	{
		friend class VoxelChunkMap;

	public:
		static const int TableWidth = 64; // 64(chunk count)
		static const int TableWidthWS = TableWidth * ChunkWidth; // world: 64(chunk count) * 16(chunk size)

	public:
		/// <summary>
		/// The root world-space X coord.
		/// </summary>
		int worldX = 0;

		/// <summary>
		/// The root world-space Z coord.
		/// </summary>
		int worldZ = 0;

		/// <summary>
		/// Add chunk to the root.
		/// </summary>
		/// <param name="chunk">The chunk.</param>
		/// <param name="x">The chunk x coord(chunk-space)</param>
		/// <param name="z">The chunk z coord(chunk-space)</param>
		void addChunk(VoxelChunk* chunk, int x, int z);

		/// <summary>
		/// Remove chunk at given coords.
		/// </summary>
		/// <param name="x">The chunk x coord(chunk-space)</param>
		/// <param name="z">The chunk z coord(chunk-space)</param>
		void removeChunk(int x, int z);

		/// <summary>
		/// Gets chunk at given coords.
		/// </summary>
		/// <param name="x">The chunk x coord(chunk-space)</param>
		/// <param name="z">The chunk z coord(chunk-space)</param>
		/// <returns>The found chunk or null when not found.</returns>
		VoxelChunk* getChunk(int x, int z);

		/// <summary>
		/// Gets chunk at given index.
		/// </summary>
		/// <param name="index">The index</param>
		/// <returns>The found chunk or null when not found.</returns>
		VoxelChunk* getChunk(int index);

	private:
		MapRoot* m_neighTop = nullptr;
		MapRoot* m_neighBottom = nullptr;
		MapRoot* m_neighLeft = nullptr;
		MapRoot* m_neighRight = nullptr;

		VoxelChunk* m_table[TableWidth * TableWidth] = {};
	};

private:
	std::vector<MapRoot*> m_roots = {};
	std::vector<VoxelChunk*> m_chunks = {};

private:
	MapRoot* findRoot(int x, int z);
	void addRoot(MapRoot* root);

public:
	/// <summary>
	/// Add chunk to the chunk map.
	/// </summary>
	/// <param name="chunk">The chunk.</param>
	void addChunk(VoxelChunk* chunk);

	/// <summary>
	/// Gets chunk at given coords.
	/// </summary>
	/// <param name="x">The chunk x coord(chunk-space)</param>
	/// <param name="z">The chunk z coord(chunk-space)</param>
	/// <returns>The found chunk or null when not found.</returns>
	VoxelChunk* findChunk(int x, int z);

	/// <summary>
	/// Gets chunks array.
	/// </summary>
	/// <returns>The chunks array.</returns>
	std::vector<VoxelChunk*>* getChunks();

	/// <summary>
	/// Draw all visible chunks
	/// </summary>
	void draw();

	/// <summary>
	/// Dispose the chunk map.
	/// </summary>
	void dispose();
};

#endif // VOXELCHUNKMAP_H
