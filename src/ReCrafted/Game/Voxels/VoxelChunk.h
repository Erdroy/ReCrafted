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
	friend class VoxelMap;
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
	bool m_hasVoxels = false;
	voxelid* m_voxels = nullptr;
	Ptr<Mesh> m_mesh = nullptr;

public:
	/// <summary>
	/// Updates the chunk.
	/// </summary>
	void update();

	/// <summary>
	/// Simulates the chunk.
	/// </summary>
	void simulate();

	/// <summary>
	/// Draws the chunk.
	/// </summary>
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

	/// <summary>
	/// Disposes the chunk.
	/// </summary>
	FORCEINLINE void dispose()
	{
		if (m_processing)
		{
			// TODO: force stop processing
			return;
		}

		m_disposed = true;

		// unset all neighs
		if (m_neighN)
			m_neighN->m_neighS = nullptr;

		if (m_neighNE)
			m_neighNE->m_neighSW = nullptr;

		if (m_neighE)
			m_neighE->m_neighW = nullptr;

		if (m_neighSE)
			m_neighSE->m_neighNW = nullptr;

		if (m_neighSE)
			m_neighSE->m_neighNW = nullptr;

		if (m_neighS)
			m_neighS->m_neighN = nullptr;

		if (m_neighSW)
			m_neighSW->m_neighNE = nullptr;

		if (m_neighW)
			m_neighW->m_neighE = nullptr;

		if (m_neighNW)
			m_neighNW->m_neighSE = nullptr;

		if(m_mesh)
			m_mesh->dispose();

		if(m_voxels != nullptr)
			delete[] m_voxels;

		delete this;
	}

	/// <summary>
	/// Generates voxel data for this chunk.
	/// </summary>
	void generateVoxelData();

	/// <summary>
	/// Generates mesh for this chunk.
	/// </summary>
	void generateMesh(
		std::vector<Vector3>* verticesArray,
		std::vector<Vector3>* normalsArray,
		std::vector<Vector2>* uvsArray,
		std::vector<Vector4>* colorsArray,
		std::vector<uint>* indicesArray);

	/// <summary>
	/// Gets voxel from THIS chunk, not using neighs.
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	/// <returns></returns>
	FORCEINLINE voxelid getVoxel(int x, int y, int z) const
	{
		if (y >= ChunkHeight || y < 0)
			return voxel_air; // out of space!
		
		if (x >= 0 && x < ChunkWidth && z >= 0 && z < ChunkWidth)
			return m_voxels[y * (ChunkWidth * ChunkWidth) + z * ChunkWidth + x]; // this chunk

		return voxel_air; // nope
	}

	/// <summary>
	/// Gets voxel from THIS chunk or neigh.
	/// This needs all neighs to be loaded!
	/// </summary>
	/// <param name="x">The x coord of voxel.</param>
	/// <param name="y">The y coord of voxel.</param>
	/// <param name="z">The z coord of voxel.</param>
	/// <returns>The voxel.</returns>
	FORCEINLINE voxelid getVoxelCC(int x, int y, int z) const
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

	/// <summary>
	/// Finds and set all not set neighs.
	/// </summary>
	void updateNeighs();

	/// <summary>
	/// Does this chunk have any neigh?
	/// </summary>
	/// <returns>True when any neigh exists.</returns>
	FORCEINLINE bool hasAnyNeigh() const
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

	/// <summary>
	/// Does this chunk have all neighs?
	/// </summary>
	/// <returns>True when all neighs exists.</returns>
	FORCEINLINE bool hasNeighs() const
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

	/// <summary>
	/// Does this chunk have all neighs loaded?
	/// </summary>
	/// <returns>True when all neighs loaded.</returns>
	FORCEINLINE bool hasLoadedNeighs() const
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

	/// <summary>
	/// The north neigh.
	/// </summary>
	FORCEINLINE VoxelChunk* neighN() const
	{
		return m_neighN;
	}
	
	/// <summary>
	/// The north-east neigh.
	/// </summary>
	FORCEINLINE VoxelChunk* neighNE() const
	{
		return m_neighNE;
	}

	/// <summary>
	/// The east neigh.
	/// </summary>
	FORCEINLINE VoxelChunk* neighE() const
	{
		return m_neighE;
	}

	/// <summary>
	/// The south-east neigh.
	/// </summary>
	FORCEINLINE VoxelChunk* neighSE() const
	{
		return m_neighSE;
	}

	/// <summary>
	/// The south neigh.
	/// </summary>
	FORCEINLINE VoxelChunk* neighS() const
	{
		return m_neighS;
	}

	/// <summary>
	/// The south-west neigh.
	/// </summary>
	FORCEINLINE VoxelChunk* neighSW() const
	{
		return m_neighSW;
	}

	/// <summary>
	/// The west neigh.
	/// </summary>
	FORCEINLINE VoxelChunk* neighW() const
	{
		return m_neighW;
	}

	/// <summary>
	/// The north-west neigh.
	/// </summary>
	FORCEINLINE VoxelChunk* neighNW() const
	{
		return m_neighNW;
	}
};

#endif // VOXELCHUNK_H
