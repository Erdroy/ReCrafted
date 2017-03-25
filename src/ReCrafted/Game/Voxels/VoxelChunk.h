// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef VOXELCHUNK_H
#define VOXELCHUNK_H

// includes
#include "../../recraftedPrerequisites.h"
#include "../../Graphics/Mesh.h"
#include "../../Graphics/Rendering.h"
#include "../../Core/Time.h"

typedef unsigned char voxelid;

const voxelid voxel_air = voxelid(0u);

static const int ChunkWidth = 16;
static const int ChunkHeight = 128;

/// <summary>
/// VoxelChunk class.
/// </summary>
class VoxelChunk
{
	friend class VoxelWorld;
	friend class VoxelChunkMap;
	friend class VoxelChunkProcessor;

private:
	int m_x = 0;
	int m_z = 0;

	volatile bool m_processing = false;
	volatile bool m_queued = false;
	volatile bool m_disposed = false;

	float m_lastTimeVisible = 0.0f;


	void* root = nullptr;
	VoxelWorld* world = nullptr;
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

	/* voxels */
	voxelid* m_voxels = nullptr;
	bool m_hasVoxels = false;

public:
	void update();

	void simulate();

	FORCEINLINE void draw()
	{
		if (m_processing || m_queued || m_mesh == nullptr)
			return;

		// check if the mesh is uploaded already
		if (!m_mesh->isUploaded() && m_mesh->canUpload())
			m_mesh->upload(); // upload all changes

		m_lastTimeVisible = Time::time();

		auto modelMatrix = Matrix::identity();
		modelMatrix.M30 = float(m_x) * ChunkWidth;
		modelMatrix.M31 = 0.0f;
		modelMatrix.M32 = float(m_z) * ChunkWidth;

		Rendering::getInstance()->draw(m_mesh, &modelMatrix);
	}

	FORCEINLINE void dispose()
	{
		if (m_processing)
		{
			// TODO: force stop processing
			return;
		}

		m_disposed = true;

		if(m_mesh)
			m_mesh->dispose();

		if(m_voxels != nullptr)
			delete[] m_voxels;

		delete this;
	}

	void worker_dataGenerate();

	void worker_meshGenerate();

	void updateNeighs();

	FORCEINLINE bool hasAnyNeigh()
	{

		return m_neighN
			|| m_neighNE
			|| m_neighE
			|| m_neighSE
			|| m_neighS
			|| m_neighSW
			|| m_neighW
			|| m_neighNW;
	}

	FORCEINLINE bool hasNeighs()
	{
		return m_neighN 
		&& m_neighNE 
		&& m_neighE
		&& m_neighSE
		&& m_neighS 
		&& m_neighSW 
		&& m_neighW 
		&& m_neighNW;
	}

	FORCEINLINE bool hasLoadedNeighs()
	{
		return m_neighN->m_voxels
			&& m_neighNE->m_voxels
			&& m_neighE->m_voxels
			&& m_neighSE->m_voxels
			&& m_neighS->m_voxels
			&& m_neighSW->m_voxels
			&& m_neighW->m_voxels
			&& m_neighNW->m_voxels;
	}

	FORCEINLINE voxelid getVoxel(int x, int y, int z)
	{
		if (y >= ChunkHeight || y < 0)
			return voxel_air; // out of space!
		
		if (x >= 0 && x < ChunkWidth && z >= 0 && z < ChunkWidth)
			return m_voxels[y * (ChunkWidth * ChunkWidth) + z * ChunkWidth + x]; // this chunk

		return voxel_air; // nope
	}

	FORCEINLINE voxelid getVoxelCC(int x, int y, int z) // CC - cross chunk
	{
		#define is_out(A) (A < 0 || A >= ChunkWidth)

		#define is_up(A) (A >= ChunkWidth)
		#define is_down(A) (A < 0)
		#define is_right(A) (A >= ChunkWidth)
		#define is_left(A) (A < 0)

		if (y >= ChunkHeight || y < 0)
			return voxel_air; // out of space!

		if (x >= 0 && x < ChunkWidth && z >= 0 && z < ChunkWidth)
			return getVoxel(x, y, z); // this chunk

		// north neigh chunk
		if (!is_out(x) && is_up(z))
			return m_neighN->getVoxel(x, y, z - ChunkWidth);

		// north east chunk
		if (is_right(x) && is_up(z))
			return m_neighNE->getVoxel(x - ChunkWidth, y, z - ChunkWidth);

		// east chunk
		if (is_right(x) && !is_out(z))
			return m_neighE->getVoxel(x - ChunkWidth, y, z);

		// south east chunk
		if (is_right(x) && is_down(z))
			return m_neighSE->getVoxel(x - ChunkWidth, y, z + ChunkWidth);

		// south chunk
		if (!is_out(x) && is_down(z))
			return m_neighS->getVoxel(x, y, z + ChunkWidth);

		// south west chunk
		if (is_left(x) && is_down(z))
			return m_neighSW->getVoxel(x + ChunkWidth, y, z + ChunkWidth);

		// west chunk
		if (is_left(x) && !is_out(z))
			return m_neighW->getVoxel(x + ChunkWidth, y, z);

		// north west chunk
		if (is_left(x) && is_up(z))
			return m_neighNW->getVoxel(x + ChunkWidth, y, z - ChunkWidth);

		#undef is_out
		#undef is_up
		#undef is_down
		#undef is_right
		#undef is_left
		return voxel_air; // nope
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
