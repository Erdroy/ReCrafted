// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef VOXELCHUNKMAP_H
#define VOXELCHUNKMAP_H

// includes
#include "VoxelChunk.h"

/// <summary>
/// VoxelMap class, holds all chunks.
/// </summary>
class VoxelMap
{
	friend class Universe;
	friend class VoxelChunk;

	class MapRoot
	{
		friend class VoxelMap;

	public:
		static const int TableWidth = 64; // 64(chunk count)
		static const int TableWidthWS = TableWidth * ChunkWidth; // world: 64(chunk count) * 16(chunk size)

	private:
		VoxelMap* m_vcm = nullptr;

	private:
		FORCEINLINE bool containsX(int value) const
		{
			return worldX <= value * ChunkWidth && worldX + TableWidthWS >= value * ChunkWidth + ChunkWidth;
		}
		FORCEINLINE bool containsZ(int value) const
		{
			return worldZ <= value * ChunkWidth && worldZ + TableWidthWS >= value * ChunkWidth + ChunkWidth;
		}
		FORCEINLINE bool contains(int x, int z) const
		{
			return containsX(x) && containsZ(z);
		}

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
	/// Draws all visible chunks
	/// </summary>
	void draw();

	/// <summary>
	/// Disposes the chunk map.
	/// </summary>
	void dispose();

	/// <summary>
	/// Converts chunk position(in chunk space) to root space position.
	/// </summary>
	/// <param name="a">The coord.</param>
	/// <returns>The root space position.</returns>
	FORCEINLINE static int chunkToRoot(int a)
	{
		return a >= 0 ? a / MapRoot::TableWidth : (a + 1) / MapRoot::TableWidth - 1;
	}
};

#endif // VOXELCHUNKMAP_H
