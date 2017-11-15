// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef MCMESHER_H
#define MCMESHER_H

// includes
#include "../IVoxelMesher.h"
#include "Core/Containers/Array.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector4.h"

class MCMesher : public IVoxelMesher
{
private:
	static MCMesher* m_instance;

private:
	Array<Vector3> m_vertices = {};
	Array<uint> m_indices = {};
	Array<Vector3> m_normals = {};
	Array<Vector4> m_colors = {};
	Array<Vector2> m_uvs = {};

public:
	virtual ~MCMesher() {}

public:
	/**
	* \brief Virtual method for generating a mesh from hermite voxel data.
	* \param mesh The mesh that will get the new mesh data.
	* \param data The hermite voxel data (in -127 to 128 range).
	* \param count The count of elements in single axis (default: 'ChunkDataSize').
	*/
	void generate(Vector3 position, float sizeMod, Ptr<Mesh>& mesh, sbyte* data, uint16_t count = SpaceObjectChunk::ChunkDataSize) override;

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
