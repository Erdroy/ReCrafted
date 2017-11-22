// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef MCMESHER_H
#define MCMESHER_H

// includes
#include "../IVoxelMesher.h"
#include "Core/Containers/Array.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector4.h"

struct TransitionCellCache;
struct RegularCellCache;

class MCMesher : public IVoxelMesher
{
private:
	struct Cell
	{
	public:
		Vector3 vertexPosition;
		uint vertexIndex;
	};

private:
	static MCMesher* m_instance;

private:
	Array<Vector3> m_vertices = {};
	Array<uint> m_indices = {};
	Array<Vector3> m_normals = {};
	Array<Vector4> m_colors = {};
	Array<Vector2> m_uvs = {};

	Cell m_cells[SpaceObjectChunk::ChunkSize * SpaceObjectChunk::ChunkSize * SpaceObjectChunk::ChunkSize] = {};

public:
	virtual ~MCMesher() {}

public:
	/**
	* \brief Virtual method for generating a mesh from hermite voxel data.
	* \param mesh The mesh that will get the new mesh data.
	* \param data The hermite voxel data (in -127 to 127 range).
	*/
	void generate(Vector3 position, int lod, Ptr<Mesh>& mesh, sbyte* data) override;

public:
	static MCMesher* getInstance()
	{
		if (m_instance)
			return m_instance;

		// create instance
		m_instance = new MCMesher;

		// done
		return m_instance;
	}
};

#endif // MCMESHER_H
