// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef VOXELCHUNKMESH_H
#define VOXELCHUNKMESH_H

// includes
#include "ReCraftedPrerequisites.h"
#include "Graphics/Mesh.h"
#include "Graphics/Graphics.h"
#include "Voxel.h"
#include "VoxelMaterialManager.h"
#include "Assets/VoxelMaterial.h"

class VoxelChunkMesh : IDisposable
{
public:
    struct MeshSection
    {
    public:
        /**
         * \brief The chunk mesh section.
         */
        RefPtr<Mesh> mesh = nullptr;

        /**
         * \brief The material set array for this section.
         * Contains max 16 voxel materials.
         */
        Array<VoxelMaterial_t> materialSet = {};

        /**
         * \brief Contains all textures that this mesh section use.
         */
        struct
        {
            Array<Renderer::Texture2DHandle> cbNear = {};
            Array<Renderer::Texture2DHandle> cbFar = {};
            Array<Renderer::Texture2DHandle> nsmNear = {};
            Array<Renderer::Texture2DHandle> nsmFar = {};
        } textures;

    public:
        void RebuildTextures()
        {
            textures.cbNear.Clear();
            textures.cbFar.Clear();
            textures.nsmNear.Clear();
            textures.nsmFar.Clear();

            for (rvar material : materialSet)
            {
                cvar cb_near = VoxelMaterialManager::GetMaterial(material)->GetTexture(VoxelMaterial::VoxelMaterialType::ColorBlend);
                cvar cb_far = VoxelMaterialManager::GetMaterial(material)->GetTexture(VoxelMaterial::VoxelMaterialType::ColorBlendFar);
                cvar nsm_near = VoxelMaterialManager::GetMaterial(material)->GetTexture(VoxelMaterial::VoxelMaterialType::NormalSmoothnessMetallic);
                cvar nsm_far = VoxelMaterialManager::GetMaterial(material)->GetTexture(VoxelMaterial::VoxelMaterialType::NormalSmoothnessMetallicFar);

                if (!cb_near)
                    break;

                textures.cbNear.Add(cb_near->GetHandle());

                // TODO: Bind default textures if none found (or use near)

                if(cb_far)
                    textures.cbFar.Add(cb_far->GetHandle());

                if(nsm_near)
                    textures.nsmNear.Add(nsm_near->GetHandle());

                if(nsm_far)
                    textures.nsmFar.Add(nsm_far->GetHandle());
            }
        }
    };

private:
    Array<MeshSection> m_meshSections;

public:
    /**
     * \brief Adds new mesh section to this chunk mesh.
     * \param section The new mesh section.
     */
    void AddSection(MeshSection& section)
    {
        // Build texture array
        section.RebuildTextures();

        // Add to array
        m_meshSections.Add(section);
    }

    /**
     * \brief Uploads all meshes using async upload function.
     */
    void Upload()
    {
        for (rvar section : m_meshSections)
        {
            DEBUG_ASSERT(section.mesh != nullptr);
            section.mesh->Upload();
        }
    }

    /**
     * \brief Uploads all meshes now using sync upload function.
     */
    void UploadNow()
    {
        for (rvar section : m_meshSections)
        {
            DEBUG_ASSERT(section.mesh != nullptr);
            section.mesh->UploadNow();
        }
    }

    /**
     * \brief Draws mesh sections.
     */
    void Draw()
    {
        for(rvar section : m_meshSections)
        {
            DEBUG_ASSERT(section.mesh != nullptr);

            // Set 'ColorBlend-Near' textures
            Graphics::GetInstance()->SetTextureArray(0, section.textures.cbNear.Data(), section.textures.cbNear.Count());

            // Draw mesh
            Graphics::GetInstance()->Draw(section.mesh);
        }
    }

    /**
     * \brief Disposes all mesh sections.
     */
    void Dispose() override
    {
        // Dispose all meshes
        for (rvar section : m_meshSections)
        {
            SafeDispose(section.mesh);
        }

        m_meshSections.Clear();
    }

public:
    const Array<MeshSection>& GetSections() const
    {
        return m_meshSections;
    }

    /**
     * \brief Returns true when empty (no sections present).
     */
    bool Empty() const
    {
        return m_meshSections.Count() == 0u;
    }
};

#endif // VOXELCHUNKMESH_H
