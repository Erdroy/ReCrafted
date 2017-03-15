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
	public:
		static const int TableWidth = 64; // 64(chunk count)
		static const int TableWidthWS = 64 * ChunkWidth; // world: 64(chunk count) * 16(chunk size)

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
		VoxelChunk* m_table[TableWidth * TableWidth] = {};
	};

private:
	std::vector<MapRoot*> m_roots = {};
	std::vector<VoxelChunk*> m_chunks = {};

private:
	MapRoot* findRoot(int x, int z);

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
	/// Get visible chunks in range from point.
	/// </summary>
	/// <param name="point">The point.</param>
	/// <param name="range">The range.</param>
	/// <param name="chunks">The output chunks array.</param>
	void getVisibleChunks(Vector2 point, float range, std::vector<VoxelChunk*>* chunks);

	/// <summary>
	/// Dispose the chunk map.
	/// </summary>
	void dispose();
};

#endif // VOXELCHUNKMAP_H
