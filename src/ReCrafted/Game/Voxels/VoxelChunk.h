// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef VOXELCHUNK_H
#define VOXELCHUNK_H

// includes
#include "../../recraftedPrerequisites.h"
#include "../../Graphics/Mesh.h"
#include "../../Graphics/Rendering.h"

typedef unsigned short voxelid;

const voxelid air = voxelid(0u);

/// <summary>
/// VoxelChunk class.
/// </summary>
class VoxelChunk
{
	friend class VoxelWorld;

private:
	static const int ChunkWidth = 16;
	static const int ChunkHeight = 256;

	VoxelWorld* world = nullptr;

	int m_x = 0;
	int m_y = 0;

	voxelid m_voxels[ChunkWidth][ChunkHeight][ChunkWidth];

	Ptr<Mesh> m_mesh = nullptr;

	/* neighbours */
	VoxelChunk* m_neighN = nullptr;
	VoxelChunk* m_neighNE = nullptr;
	VoxelChunk* m_neighE = nullptr;
	VoxelChunk* m_neighSE = nullptr;
	VoxelChunk* m_neighS = nullptr;
	VoxelChunk* m_neighSW = nullptr;
	VoxelChunk* m_neighW = nullptr;
	VoxelChunk* m_neighNW = nullptr;

private:
	void worker_dataGenerate();

	void worker_meshGenerate();

	FORCEINLINE void dataGenerate()
	{
		worker_dataGenerate();
	}

	FORCEINLINE void meshGenerate()
	{
		worker_meshGenerate();
	}

	FORCEINLINE void meshUpload() const
	{
		m_mesh->applyChanges();
	}

public:

	void update();

	void simulate();

	FORCEINLINE void draw() const
	{
		if (m_mesh == nullptr)
			return;

		auto modelMatrix = Matrix::identity();
		Rendering::getInstance()->draw(m_mesh, &modelMatrix);
	}

	FORCEINLINE void dispose() const
	{
		if(m_mesh)
			m_mesh->dispose();
	}

	FORCEINLINE voxelid getVoxel(int x, int y, int z)
	{
		if (y >= ChunkWidth || y < 0)
			return air; // out of space!
		
		if (x >= 0 && x < ChunkWidth && z >= 0 && z < ChunkWidth)
			return m_voxels[x][y][z]; // this chunk

		return air; // nope
	}

	FORCEINLINE voxelid getVoxelCC(int x, int y, int z) // CC - cross chunk
	{
		if (y >= ChunkWidth || y < 0)
			return air; // out of space!

		if (x >= 0 && x < ChunkWidth && z >= 0 && z < ChunkWidth)
			return m_voxels[x][y][z]; // this chunk

		// north neigh chunk
		if (x < 0 && z >= 0 && z < ChunkWidth)
			return m_neighN->getVoxel(x + ChunkWidth, y, z);

		// TODO: rest of chunks

		return air; // nope
	}

	// recursive tree methods
	FORCEINLINE VoxelChunk* neighN() const
	{
		return m_neighN;
	}
	FORCEINLINE VoxelChunk* neighNE() const
	{
		return m_neighNE;
	}
	FORCEINLINE VoxelChunk* neighE() const
	{
		return m_neighE;
	}
	FORCEINLINE VoxelChunk* neighSE() const
	{
		return m_neighSE;
	}
	FORCEINLINE VoxelChunk* neighS() const
	{
		return m_neighS;
	}
	FORCEINLINE VoxelChunk* neighSW() const
	{
		return m_neighSW;
	}
	FORCEINLINE VoxelChunk* neighW() const
	{
		return m_neighW;
	}
	FORCEINLINE VoxelChunk* neighNW() const
	{
		return m_neighNW;
	}
};

#endif // VOXELCHUNK_H
