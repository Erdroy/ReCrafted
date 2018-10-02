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
#include "../CommonTables.hpp"

#include <bitset>

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

    /**
    * \brief Enables or disables transition cell generation.
    */
    static const bool EnableTransitionCells = false;

    static const int VertexReuseEntriesCount = (VoxelChunkData::ChunkDataSize * 3) * (VoxelChunkData::ChunkDataSize * 3) * (VoxelChunkData::ChunkDataSize * 3);

private:
    struct MeshSection
    {
    public:
        std::bitset<VertexReuseEntriesCount> vertexReuseMap;
        Array<int> vertexReuse;

        Array<uint> vertexIds;
        Array<Vector3> vertices;
        Array<Vector3> normals;
        Array<Vector4> colors;
        Array<Vector4> materialsA;
        Array<Vector4> materialsB;
        Array<Vector4> materialsC;
        Array<Vector4> materialsD;
        Array<uint> indices;
        Array<VoxelMaterial_t> materialSet;

    public:
        MeshSection() : 
            vertexReuse({}), 
            vertexIds({}),
            vertices({}), 
            normals({}), 
            colors({}), 
            materialsA({}), 
            materialsB({}),
            materialsC({}),
            materialsD({}), 
            indices({})
        {  
            // Initialize vertex reuse
            vertexReuse.Resize(VertexReuseEntriesCount);

            // Reserve a bit of space right now.
            vertexIds.Reserve(4096);
            vertices.Reserve(4096);
            normals.Reserve(4096);
            materialsA.Reserve(4096);
            materialsB.Reserve(4096);
            materialsC.Reserve(4096);
            materialsD.Reserve(4096);
            indices.Reserve(4096);

            // Reset reuse map
            vertexReuseMap.reset();
        }

    public:
        void AddVertex(const VertexInfo& vertexInfo, bool normalCorrection)
        {
            var index = vertices.Count();

            if(!vertexReuseMap.test(vertexInfo.vertexId))
            {
                vertexIds.Add(vertexInfo.vertexId);
                vertices.Add(vertexInfo.vertexPosition);
                normals.Add(vertexInfo.vertexNormal);
            }
            else
            {
                index = vertexInfo.vertexId;
            }

            if(materialSet.Size() == 1)
            {
                materialsA.Add(EncodedMaterials[0]);
                materialsB.Add(Vector4::Zero());
                materialsC.Add(Vector4::Zero());
                materialsD.Add(Vector4::Zero());
            }
            else
            {
                // Encode material
                int materialChannel;
                cvar material = EncodeMaterial(materialSet, vertexInfo.voxelMaterial, &materialChannel);

                // Write material
                switch (materialChannel)
                {
                case 0:
                    materialsA.Add(material);
                    materialsB.Add(Vector4::Zero());
                    materialsC.Add(Vector4::Zero());
                    materialsD.Add(Vector4::Zero());
                    break;
                case 1:
                    materialsA.Add(Vector4::Zero());
                    materialsB.Add(material);
                    materialsC.Add(Vector4::Zero());
                    materialsD.Add(Vector4::Zero());
                    break;
                case 2:
                    materialsA.Add(Vector4::Zero());
                    materialsB.Add(Vector4::Zero());
                    materialsC.Add(material);
                    materialsD.Add(Vector4::Zero());
                    break;
                case 3:
                    materialsA.Add(Vector4::Zero());
                    materialsB.Add(Vector4::Zero());
                    materialsC.Add(Vector4::Zero());
                    materialsD.Add(material);
                    break;
                }
            }

            // Add index when this is not normal correction vertex
            if (!normalCorrection)
                indices.Add(index);
        }

        void AddMaterial(const VoxelMaterial_t material)
        {
            if (HasMaterial(material))
                return;

            materialSet.Add(material);
        }

        bool MaterialsEmpty() const
        {
            return materialSet.Empty();
        }

        bool MaterialsFull() const
        {
            return materialSet.Count() == MaxMaterialsPerSection;
        }

        bool CanFit(const VoxelMaterial_t materialA, const VoxelMaterial_t materialB, const VoxelMaterial_t materialC) const
        {
            if (MaterialsEmpty())
                return true;

            if (MaterialsFull())
                return false;

            // Calculate needed space
            var neededEntries = 0u;

            if (!HasMaterial(materialA))
                neededEntries++;
            if (!HasMaterial(materialB))
                neededEntries++;
            if (!HasMaterial(materialC))
                neededEntries++;

            return MaxMaterialsPerSection - materialSet.Count() >= neededEntries;
        }

        /**
        * \brief Returns true when this material set already contains given material.
        * \param material The material.
        */
        bool HasMaterial(const VoxelMaterial_t material) const
        {
            return materialSet.Contains(material);
        }

        /**
        * \brief Returns true when this material set already contains given material.
        * \param materialA The first material.
        * \param materialB The second material.
        * \param materialC The third material.
        */
        bool HasMaterials(const VoxelMaterial_t materialA, const VoxelMaterial_t materialB, const VoxelMaterial_t materialC) const
        {
            if (materialA == materialB && materialB == materialC)
                return HasMaterial(materialA);

            return HasMaterial(materialA) && HasMaterial(materialB) && HasMaterial(materialC);
        }

        void Clear()
        {
            // Clear all arrays
            vertexIds.Clear();
            vertices.Clear();
            normals.Clear();
            colors.Clear();
            materialsA.Clear();
            materialsB.Clear();
            materialsC.Clear();
            materialsD.Clear();
            indices.Clear();

            // Clear material set
            materialSet.Clear();

            // Reset reuse map
            vertexReuseMap.reset();
        }
    };

private:
    uint32_t m_triangleCount;
    int m_currentSection;
    Array<MeshSection> m_meshSections;

    Array<VertexInfo> m_vertexInfo;
    std::bitset<VertexReuseEntriesCount> m_vertexInfoMap;

public:
    TransvoxelMesher()
    {
        m_triangleCount = 0u;
        m_currentSection = 0;

        for (var i = 0; i < MaxSections; i++)
            m_meshSections.Add(MeshSection());

        // Initialize vertex info array
        // Reserve space for vertex info array
        m_vertexInfo.Resize(VertexReuseEntriesCount);

        // Reset vertex map
        m_vertexInfoMap.reset();
    }

    virtual ~TransvoxelMesher() = default;

private:
    MeshSection & GetSection()
    {
        return m_meshSections[m_currentSection];
    }

    MeshSection* FindSection(VoxelMaterial_t material);
    MeshSection* FindSection(VoxelMaterial_t materialA, VoxelMaterial_t materialB, VoxelMaterial_t materialC);
    MeshSection* PushSection(VoxelMaterial_t materialA, VoxelMaterial_t materialB, VoxelMaterial_t materialC);
    MeshSection* FindOrPushSection(VoxelMaterial_t materialA, VoxelMaterial_t materialB, VoxelMaterial_t materialC);
    MeshSection* CreateSection();

    void AddTriangle(const VertexInfo& vertexInfoA, const VertexInfo& vertexInfoB, const VertexInfo& vertexInfoC, bool normalCorrection);

private:
    void PolygonizeRegularCell(const Vector3& position, Voxel* data, float voxelScale, int lod, const Int3& voxelOffset, bool normalCorrection);

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
        return m_triangleCount > 0u;
    }

    /**
    * \brief Uploads all data to a mesh. This also clears the mesher and prepares to next mesh generation.
    * \param chunkMesh The mesh that will get the new mesh data.
    */
    void Apply(const RefPtr<VoxelChunkMesh>& chunkMesh) override;

    /**
    * \brief Cleans all data used during Generate and Apply functions.
    */
    void Clear() override;
};

#endif // TRANSVOXELMESHER_H
