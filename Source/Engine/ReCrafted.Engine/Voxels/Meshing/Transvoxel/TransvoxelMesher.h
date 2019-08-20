// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/List.h"

#include "Voxels/Meshing/IVoxelMesher.h"
#include "Voxels/Meshing/MeshingHelpers.h"
#include "Voxels/Meshing/MaterialHelpers.h"
#include "Voxels/Meshing/CommonTables.hpp"

#include <bitset>

/// <summary>
///     The voxel mesher implementation class of Transvoxel algorithm.
/// </summary>
class TransvoxelMesher final : public IVoxelMesher
{
    DELETE_COPY_MOVE(TransvoxelMesher);

private:
    /// <summary>
    ///     The maximal amount of materials per mesh section.
    /// </summary>
    static const int MaxMaterialsPerSection = 16;

    /// <summary>
    ///     The maximal amount of mesh section.
    /// </summary>
    static const int MaxSections = 8;

    /// <summary>
    ///     Enables or disables transition cell generation.
    /// </summary>
    static const bool EnableTransitionCells = false;

    static const int VertexReuseEntriesCount = (VoxelChunkData::ChunkDataSize * 3) * (VoxelChunkData::ChunkDataSize * 3) * (VoxelChunkData::ChunkDataSize * 3);

private:
    struct MeshSection
    {
    public:
        std::bitset<VertexReuseEntriesCount> vertexReuseMap;
        List<int> vertexReuse;

        List<uint> vertexIds;
        List<Vector3> vertices;
        List<Vector3> normals;
        List<Vector4> colors;
        List<Vector4> materialsA;
        List<Vector4> materialsB;
        List<Vector4> materialsC;
        List<Vector4> materialsD;
        List<uint> indices;
        List<VoxelMaterial_t> materialSet;

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
            auto index = static_cast<uint>(vertices.Count());

            if (!vertexReuseMap.test(vertexInfo.vertexId))
            {
                vertexIds.Add(vertexInfo.vertexId);
                vertices.Add(vertexInfo.vertexPosition);
                normals.Add(vertexInfo.vertexNormal);
            }
            else
            {
                index = vertexInfo.vertexId;
            }

            if (materialSet.Size() == 1)
            {
                materialsA.Add(EncodedMaterials[0]);
                materialsB.Add(Vector4::Zero);
                materialsC.Add(Vector4::Zero);
                materialsD.Add(Vector4::Zero);
            }
            else
            {
                // Encode material
                int materialChannel;
                const auto material = EncodeMaterial(materialSet, vertexInfo.voxelMaterial, &materialChannel);

                // Write material
                switch (materialChannel)
                {
                case 0:
                    materialsA.Add(material);
                    materialsB.Add(Vector4::Zero);
                    materialsC.Add(Vector4::Zero);
                    materialsD.Add(Vector4::Zero);
                    break;
                case 1:
                    materialsA.Add(Vector4::Zero);
                    materialsB.Add(material);
                    materialsC.Add(Vector4::Zero);
                    materialsD.Add(Vector4::Zero);
                    break;
                case 2:
                    materialsA.Add(Vector4::Zero);
                    materialsB.Add(Vector4::Zero);
                    materialsC.Add(material);
                    materialsD.Add(Vector4::Zero);
                    break;
                case 3:
                    materialsA.Add(Vector4::Zero);
                    materialsB.Add(Vector4::Zero);
                    materialsC.Add(Vector4::Zero);
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
            auto neededEntries = 0u;

            if (!HasMaterial(materialA))
                neededEntries++;
            if (!HasMaterial(materialB))
                neededEntries++;
            if (!HasMaterial(materialC))
                neededEntries++;

            return MaxMaterialsPerSection - materialSet.Count() >= neededEntries;
        }

        /// <summary>
        ///     Returns true when this material set already contains given material.
        /// </summary>
        /// <param name="material">The material.</param>
        bool HasMaterial(const VoxelMaterial_t material) const
        {
            return materialSet.Contains(material);
        }

        /// <summary>
        ///     Returns true when this material set already contains given material.
        /// </summary>
        /// <param name="materialA">The first material.</param>
        /// <param name="materialB">The second material.</param>
        /// <param name="materialC">The third material.</param>
        /// <returns></returns>
        bool HasMaterials(const VoxelMaterial_t materialA, const VoxelMaterial_t materialB, const VoxelMaterial_t materialC) const
        {
            if (materialA == materialB && materialB == materialC)
                return HasMaterial(materialA);

            return HasMaterial(materialA) && HasMaterial(materialB) && HasMaterial(materialC);
        }

        /// <summary>
        ///     Clears all cached data.
        /// </summary>
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
    float m_currentVoxelScale;
    List<MeshSection> m_meshSections;

    List<VertexInfo> m_vertexInfo;
    std::bitset<VertexReuseEntriesCount> m_vertexInfoMap;

    List<Vector3> m_collisionVertices;
    List<uint> m_collisionIndices;

public:
    TransvoxelMesher()
    {
        m_triangleCount = 0u;
        m_currentSection = 0;
        m_currentVoxelScale = 1.0f;

        for (auto i = 0; i < MaxSections; i++)
            m_meshSections.Add(MeshSection());

        // Initialize vertex info array
        // Reserve space for vertex info array
        m_vertexInfo.Resize(VertexReuseEntriesCount);

        // Reset vertex map
        m_vertexInfoMap.reset();

        // Reserve some space for collision data
        m_collisionVertices.Reserve(4096);
        m_collisionIndices.Reserve(4096);
    }

    virtual ~TransvoxelMesher() = default;

private:
    MeshSection& GetSection()
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
    void PolygonizeRegularCell(const Vector3& position, Voxel* data, float voxelScale, int lod, const Vector3i& voxelOffset, bool normalCorrection);

public:
    /// <summary>
    ///     Virtual method for generating a mesh from hermite voxel data.
    /// </summary>
    /// <param name="position">The position of current processed chunk.</param>
    /// <param name="lod">The current processed chunk lod.</param>
    /// <param name="borders">Border set for current processed chunk.</param>
    /// <param name="data">The hermite voxel data (in -127 to 127 range).</param>
    void Generate(const Vector3& position, int lod, uint8_t borders, Voxel* data) override;

    /// <summary>
    ///     Checks if there are any generated triangles.
    /// </summary>
    /// <returns>True when there is at least one triangle.</returns>
    bool HasTriangles() override
    {
        return m_triangleCount > 0u;
    }

    /// <summary>
    ///     Uploads all data to a mesh. This also clears the mesher and prepares to next mesh generation.
    /// </summary>
    /// <param name="chunkMesh">The mesh that will get the new mesh data.</param>
    /// <param name="chunkCollision">The collision that will get the new collision data.</param>
    void Apply(const RefPtr<VoxelChunkMesh>& chunkMesh, MeshCollider* chunkCollision) override;

    /// <summary>
    ///     Cleans all data used during Generate and Apply functions.
    /// </summary>
    void Clear() override;

public:
    /// <summary>
    ///     Gets the name of mesher algorithm.  
    /// </summary>
    const char* GetName() override
    {
        return "Transvoxel";
    }
};
