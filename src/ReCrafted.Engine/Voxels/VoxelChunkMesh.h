// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef VOXELCHUNKMESH_H
#define VOXELCHUNKMESH_H

// includes
#include "ReCraftedPrerequisites.h"
#include "Graphics/Mesh.h"
#include "Graphics/Graphics.h"
#include "Voxel.h"

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
    };

private:
    Array<MeshSection> m_meshSections;

public:
    /**
     * \brief Adds new mesh section to this chunk mesh.
     * \param section The new mesh section.
     */
    void AddSection(const MeshSection& section)
    {
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

            var slot = 0;
            for(var material : section.materialSet)
            {
                Graphics::GetInstance()->SetTexture(slot, SpaceObject::current->m_textures[material]);
                slot++;
            }

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
    /**
     * \brief Returns true when empty (no sections present).
     */
    bool Empty() const
    {
        return m_meshSections.Count() == 0u;
    }
};

#endif // VOXELCHUNKMESH_H
