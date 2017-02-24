// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef VOXELCHUNK_H
#define VOXELCHUNK_H

// includes
#include "../../recraftedPrerequisites.h"
#include "../../Graphics/Mesh.h"
#include "../../Graphics/Rendering.h"

typedef unsigned short voxelid;

/// <summary>
/// VoxelChunk class.
/// </summary>
class VoxelChunk
{
	friend class VoxelWorld;

private:
	static const int ChunkWidth = 16;
	static const int ChunkHeight = 256;

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

	// recursive tree methods
	FORCEINLINE VoxelChunk* neighN()
	{
		
	}
	FORCEINLINE VoxelChunk* neighNE()
	{

	}
	FORCEINLINE VoxelChunk* neighE()
	{

	}
	FORCEINLINE VoxelChunk* neighSE()
	{

	}
	FORCEINLINE VoxelChunk* neighS()
	{

	}
	FORCEINLINE VoxelChunk* neighSW()
	{

	}
	FORCEINLINE VoxelChunk* neighW()
	{

	}
	FORCEINLINE VoxelChunk* neighNW()
	{

	}
};

#endif // VOXELCHUNK_H
