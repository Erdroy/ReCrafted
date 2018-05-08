// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef MCMESHER_H
#define MCMESHER_H

// includes
#include "../IVoxelMesher.h"
#include "Core/Containers/Array.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector4.h"

#define BORDER_FRONT	0x01
#define BORDER_BACK		0x02
#define BORDER_LEFT		0x04
#define BORDER_RIGHT	0x08
#define BORDER_TOP		0x10
#define BORDER_BOTTOM	0x20

struct TransitionCellCache;
struct RegularCellCache;

class MCMesher : public IVoxelMesher
{
private:
	ALIGN(4) struct Cell
	{
	public:
		//Vector3 vertexPosition;
		//uint vertexIndex;
		Vector3 vertexNormal;

		byte caseIndex;
		bool isFullOrEmpty;
	};

private:
	Array<Vector3> m_vertices = {};
	Array<uint> m_indices = {};
	Array<Vector3> m_normals = {};
	Array<Vector4> m_colors = {};

	Cell m_cells[VoxelChunkData::ChunkSize * VoxelChunkData::ChunkSize * VoxelChunkData::ChunkSize] = {};

public:
    MCMesher()
    {
        m_vertices.reserve(16 << 10);
        m_indices.reserve(16 << 10);
        m_normals.reserve(16 << 10);
        m_colors.reserve(16 << 10);
    }
	virtual ~MCMesher() {}

private:
	FORCEINLINE void clean();

	FORCEINLINE void generateCell(Cell* cell, int x, int y, int z, sbyte* data) const;
	void generateCube(Cell* cell, const Vector3& position, const Vector3& offset, float lod, sbyte* data);
	void generateSkirt(Cell* cell, const Vector3& position, const Vector3& offset, float lod, uint8_t axis, sbyte* data);
	
	FORCEINLINE void generateCells(sbyte* data, const Vector3& position, float lod, uint8_t borders);

public:
	/**
	* \brief Virtual method for generating a mesh from hermite voxel data.
	* \param mesh The mesh that will get the new mesh data.
	* \param data The hermite voxel data (in -127 to 127 range).
	*/
	void generate(const Vector3& position, int lod, uint8_t borders, Ref<Mesh>& mesh, sbyte* data) override;
};

#endif // MCMESHER_H
