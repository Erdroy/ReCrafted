// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef MCMESHER_H
#define MCMESHER_H

// includes
#include "../IVoxelMesher.h"
#include "Core/Containers/Array.h"
#include "Voxels/Storage/VoxelChunkData.h"
#include "Core/Math/Math.h"

class MCMesher : public IVoxelMesher
{
private:
    ALIGN(4) struct Cell
    {
    public:
        //Vector3 vertexPosition;
        //uint vertexIndex;
        Vector3 vertexNormal = {};

        byte caseIndex = 0u;
        bool isFullOrEmpty = true;
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
        m_vertices.Reserve(16 << 10);
        m_indices.Reserve(16 << 10);
        m_normals.Reserve(16 << 10);
        m_colors.Reserve(16 << 10);
    }

    virtual ~MCMesher()
    {
    }

private:
    FORCEINLINE void GenerateCell(Cell* cell, const Int3& offset, sbyte* data) const;
    FORCEINLINE void GenerateCube(Cell* cell, const Vector3& position, const Vector3& offset, float lod, sbyte* data);
    FORCEINLINE void GenerateSkirt(Cell* cell, const Vector3& position, const Vector3& offset, float lod, uint8_t axis, sbyte* data);

    FORCEINLINE void GenerateCells(sbyte* data, const Vector3& position, float lod, uint8_t borders);

public:
    /**
    * \brief Virtual method for generating a mesh from hermite voxel data.
    * \param data The hermite voxel data (in -127 to 127 range).
    */
    void Generate(const Vector3& position, int lod, uint8_t borders, sbyte* data) override;

    /**
    * \brief Checks if there are any generated triangles.
    * \return True when there is at least one triangle.
    */
    bool HasTriangles() override
    {
        return m_indices.Count() >= 3;
    }

    /**
    * \brief Uploads all data to a mesh. This also clears the mesher and prepares to next mesh generation.
    * \param mesh The mesh that will get the new mesh data.
    */
    void Apply(const RefPtr<Mesh>& mesh) override;

    /**
    * \brief Cleans all data used during Generate and Apply functions.
    */
    void Clear() override;
};

#endif // MCMESHER_H
