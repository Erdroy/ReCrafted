// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef IVOXELMESHER_H
#define IVOXELMESHER_H

// includes
#include "ReCrafted.h"
#include "Core/Defines.h"
#include "Game/World/SpaceObject/SpaceObjectChunk.h"

class Mesh;

/**
 * \brief The voxel mesher interface class.
 */
interface IVoxelMesher
{
public:
	/**
	 * \brief Virtual method for generating a mesh from hermite voxel data.
	 * \param mesh The mesh that will get the new mesh data.
	 * \param data The hermite voxel data (in -127 to 127 range).
	 */
	virtual void generate(const Vector3& position, int lod, uint8_t borders, Ptr<Mesh>& mesh, sbyte* data) = 0;
};

#endif // IMESHER_H
