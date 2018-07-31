// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef TRANSVOXELMESHER_H
#define TRANSVOXELMESHER_H

// includes
#include "ReCrafted.h"

#include "Core/Containers/Array.h"

#include "../IVoxelMesher.h"

class TransvoxelMesher : public IVoxelMesher
{
private: 
    // Configuration
    /**
     * \brief Enables or disables the vertex reuse.
     */
    static const bool EnableVertexReuse = true;

    /**
    * \brief Enables or disables color channel generation.
    */
    static const bool EnableColorChannelGeneration = true;

    /**
    * \brief Enables or disables material channel generation.
    */
    static const bool EnableMaterialChannelGeneration = true;

    /**
    * \brief Enables or disables skirt cell generation.
    */
    static const bool EnableSkirtCells = false;

    /**
    * \brief Enables or disables transition cell generation.
    */
    static const bool EnableTransitionCells = false;

private:
    Array<int> m_vertexReuse;

    Array<Vector3> m_vertices;
    Array<Vector3> m_normals;
    Array<Vector4> m_colors;
    Array<uint64_t> m_materials;
    Array<uint> m_indices;

public:
    TransvoxelMesher() : m_vertexReuse({}), m_vertices({}), m_normals({}),  m_colors({}), m_materials({}), m_indices({})
    {
        // ReSharper disable CppUnreachableCode
        if (EnableVertexReuse) 
        {
            // Reserve space for vertex reuse
            cvar vertexReuseSize = Math::Pow(VoxelChunkData::ChunkDataSize * 3, 3);
            m_vertexReuse.Reserve(vertexReuseSize);

            // Initialize vertex reuse
            for (var i = 0; i < vertexReuseSize; i++)
                m_vertexReuse.Add(-1);
        }

        // Reserve a bit of space right now.
        m_vertices.Reserve(16 << 10);
        m_normals.Reserve(16 << 10);

        if(EnableColorChannelGeneration)
        {
            m_colors.Reserve(16 << 10);
        }

        if(EnableMaterialChannelGeneration)
        {
            m_materials.Reserve(16 << 10);
        }

        m_indices.Reserve(16 << 10);

        // ReSharper restore CppUnreachableCode
    }

    virtual ~TransvoxelMesher() = default;

private:
    void PolygonizeRegularCell(const Vector3& position, Voxel* data, float voxelScale, const Int3& voxelOffset, bool normalCorrection);

public:
    /**
    * \brief Virtual method for generating a mesh from hermite voxel data.
    * \param position The base position of the chunk. TODO: Remove this feature, and update MVP per draw.
    * \param lod
    * \param borders
    * \param data The hermite voxel data (in -127 to 127 range).
    */
    void Generate(const Vector3& position, int lod, uint8_t borders, Voxel* data) override;

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

#endif // TRANSVOXELMESHER_H
