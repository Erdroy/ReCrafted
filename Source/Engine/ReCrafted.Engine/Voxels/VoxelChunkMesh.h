// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/List.h"
#include "Rendering/Mesh.h"
#include "Voxels/Voxel.h"
#include "Scripting/Object.h"
#include "VoxelMaterialManager.h"

class VoxelChunkMesh
{
public:
    struct MeshSection
    {
    public:
        /// <summary>
        ///     The chunk mesh section.
        /// </summary>
        Mesh* mesh = nullptr;

        /// <summary>
        ///     The material set array for this section. Contains max 16 voxel materials.
        /// </summary>
        List<VoxelMaterial_t> materialSet = {};

        /// <summary>
        ///     Contains all textures that this mesh section use.
        /// </summary>
        struct
        {
            List<Texture*> cbNear = {};
            List<Texture*> cbFar = {};
            List<Texture*> nsmNear = {};
            List<Texture*> nsmFar = {};
        } textures;

    public:
        void RebuildTextures()
        {
            textures.cbNear.Clear();
            textures.cbFar.Clear();
            textures.nsmNear.Clear();
            textures.nsmFar.Clear();

            for (auto& material : materialSet)
            {
                const auto cb_near = VoxelMaterialManager::GetMaterial(material)->GetTexture(VoxelMaterialType::ColorBlend);
                const auto cb_far = VoxelMaterialManager::GetMaterial(material)->GetTexture(VoxelMaterialType::ColorBlendFar);
                const auto nsm_near = VoxelMaterialManager::GetMaterial(material)->GetTexture(VoxelMaterialType::NormalSmoothnessMetallic);
                const auto nsm_far = VoxelMaterialManager::GetMaterial(material)->GetTexture(VoxelMaterialType::NormalSmoothnessMetallicFar);

                if (!cb_near)
                    break;

                textures.cbNear.Add(cb_near);

                // TODO: Bind default textures if none found (or use near)

                if (cb_far)
                    textures.cbFar.Add(cb_far);

                if (nsm_near)
                    textures.nsmNear.Add(nsm_near);

                if (nsm_far)
                    textures.nsmFar.Add(nsm_far);
            }
        }
    };

private:
    List<MeshSection> m_meshSections;


public:
    VoxelChunkMesh() = default;
    
    ~VoxelChunkMesh()
    {
        // Dispose all meshes
        for (auto& section : m_meshSections)
        {
            Object::Destroy(section.mesh);
        }

        m_meshSections.Clear();
    }

public:
    /// <summary>
    ///     Adds new mesh section to this chunk mesh.
    /// </summary>
    /// <param name="section">The new mesh section.</param>
    void AddSection(MeshSection& section)
    {
        // Build texture array
        section.RebuildTextures();

        // Add to array
        m_meshSections.Add(section);
    }

    /// <summary>
    ///     Uploads all meshes using async upload function.
    /// </summary>
    void Upload()
    {
        for (auto& section : m_meshSections)
        {
            DEBUG_ASSERT(section.mesh != nullptr);
            section.mesh->Upload();
        }
    }

    /// <summary>
    ///     Uploads all meshes now using sync upload function.
    /// </summary>
    void UploadNow()
    {
        for (auto& section : m_meshSections)
        {
            DEBUG_ASSERT(section.mesh != nullptr);
            section.mesh->Upload(false);
        }
    }

public:
    /// <summary>
    ///     Return true when valid.
    /// </summary>
    bool IsValid() const
    {
        return !m_meshSections.Empty();
    }

    /// <summary>
    ///     Returns the list of sections from this chunk mesh instance.
    /// </summary>
    const List<MeshSection>& GetSections() const
    {
        return m_meshSections;
    }

    /// <summary>
    ///     Returns true when empty (no sections present).
    /// </summary>
    bool Empty() const
    {
        return m_meshSections.Count() == 0u;
    }
};
