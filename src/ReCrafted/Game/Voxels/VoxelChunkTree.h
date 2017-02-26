// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef VOXELCHUNKTREE_H
#define VOXELCHUNKTREE_H

// includes
#include <vector>
#include "VoxelChunk.h"
#include "../../Graphics/Camera.h"

class VoxelChunkTree
{
	const int m_depthTable[6] = { 16384, 4096, 1024, 256, 64, 16 };

private:
	struct vctEntry
	{
		int StartX;
		int StartZ;

		// x-
		// --
		vctEntry* M00;

		// -x
		// --
		vctEntry* M01;

		// --
		// -x
		vctEntry* M11;

		// --
		// x-
		vctEntry* M10;
	};

private:
	/* 0 - 16384, 1 - 4096, 2 - 1024, 3 - 256, 4 - 64, 5 - 16*/
	std::vector<vctEntry*> m_roots = {};

public:
	void add(VoxelChunk chunk)
	{
		vctEntry* root;
		if(m_roots.size() == 0)
		{
			// initialize root
			root = new vctEntry;

			root->StartX = 0;
			root->StartZ = 0;

			m_roots.push_back(root);
		}
	}

	void getNearChunks(Vector3 point, float distance)
	{
		
	}

	void getNearChunksCulled(Vector3 point, float distance, Camera* camera)
	{

	}
};

#endif // VOXELCHUNKTREE_H