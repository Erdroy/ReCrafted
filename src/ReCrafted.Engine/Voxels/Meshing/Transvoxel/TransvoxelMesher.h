// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef TRANSVOXELMESHER_H
#define TRANSVOXELMESHER_H

// includes
#include "ReCrafted.h"

#include "Core/Containers/Array.h"

#include "../IVoxelMesher.h"
#include "../MeshingHelpers.h"
#include "../MaterialHelpers.h"

class TransvoxelMesher : public IVoxelMesher
{
private: 
    /**
     * \brief The maximal amount of materials per mesh section.
     */
    static const int MaxMaterialsPerSection = 16;

    /**
    * \brief The maximal amount of mesh section.
    */
    static const int MaxSections = 8;

    // Configuration
    /**
     * \brief Enables or disables the vertex reuse.
     */
    static const bool EnableVertexReuse = true;

    /**
    * \brief Enables or disables skirt cell generation.
    */
    static const bool EnableSkirtCells = false;

    /**
    * \brief Enables or disables transition cell generation.
    */
    static const bool EnableTransitionCells = false;

private:
    struct MeshSection
    {
    public:
        Array<int> vertexReuse;
        Array<Vector3> vertices;
        Array<Vector3> normals;
        Array<Vector4> colors;
        Array<Vector4> materials;
        Array<uint> indices;
        Array<VoxelMaterial_t> materialSet;

    public:
        MeshSection() : vertexReuse({}), vertices({}), normals({}), colors({}), materials({}), indices({})
        {
            if (EnableVertexReuse)
            {
                // Reserve space for vertex reuse
                cvar vertexReuseSize = Math::Pow(VoxelChunkData::ChunkDataSize * 3, 3);
                vertexReuse.Reserve(vertexReuseSize);

                // Initialize vertex reuse
                for (var i = 0; i < vertexReuseSize; i++)
                    vertexReuse.Add(-1);
            }

            // Reserve a bit of space right now.
            vertices.Reserve(16 << 10);
            normals.Reserve(16 << 10);
            materials.Reserve(16 << 10);
            indices.Reserve(16 << 10);
        }

    public:
        bool MaterialsEmpty() const
        {
            return materialSet.Count() == 0u;
        }

        bool MaterialsFull() const
        {
            return materialSet.Count() == MaxMaterialsPerSection;
        }

        /**
        * \brief Returns true when this material set already contains given material.
        * \param material The material.
        */
        bool HasMaterial(const VoxelMaterial_t material)
        {
            return materialSet.Contains(material);
        }

        void Clear()
        {
            // Clean vertex reuse array if reuse is enabled
            if (EnableVertexReuse)
            {
                for (var i = 0u; i < vertexReuse.Count(); i++)
                    vertexReuse[i] = -1;
            }

            // Clear all arrays
            vertices.Clear();
            normals.Clear();
            colors.Clear();
            materials.Clear();
            indices.Clear();
            materialSet.Clear();
        }
    };

private:
    uint32_t m_triangleCount;
    int m_currentSection;
    Array<MeshSection> m_meshSections;

public:
    TransvoxelMesher()
    {
        m_triangleCount = 0u;
        m_currentSection = 0;

        for (var i = 0; i < MaxSections; i++)
            m_meshSections.Add(MeshSection());
    }

    virtual ~TransvoxelMesher() = default;

private:
    void PolygonizeRegularCell(const Vector3& position, Voxel* data, float voxelScale, int lod, const Int3& voxelOffset, bool normalCorrection);

    MeshSection& GetSection()
    {
        return m_meshSections[m_currentSection];
    }

    MeshSection& CreateSection()
    {
        m_currentSection++;
        ASSERT(m_currentSection < MaxSections); // TODO: Handle error
        return GetSection();
    }

public:
    /**
    * \brief Virtual method for generating a mesh from hermite voxel data.
    * \param position The position of current procesed chunk.
    * \param lod The current procesed chunk lod.
    * \param borders Border set for current procesed chunk.
    * \param data The hermite voxel data (in -127 to 127 range).
    */
    void Generate(const Vector3& position, int lod, uint8_t borders, Voxel* data) override;

    /**
    * \brief Checks if there are any generated triangles.
    * \return True when there is at least one triangle.
    */
    bool HasTriangles() override
    {
        return m_triangleCount >= 3u;
    }

    /**
    * \brief Uploads all data to a mesh. This also clears the mesher and prepares to next mesh generation.
    * \param mesh The mesh that will get the new mesh data.
    */
    void Apply(const RefPtr<VoxelChunkMesh>& mesh) override;

    /**
    * \brief Cleans all data used during Generate and Apply functions.
    */
    void Clear() override;
};

#endif // TRANSVOXELMESHER_H
