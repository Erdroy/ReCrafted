// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef MESH_H
#define MESH_H

// includes
#include "Core/Types.h"
#include "Core/Math/Math.h"
#include "IResource.h"
#include "Renderer/Renderer.hpp"

struct Vector4;

/**
 * \brief Mesh class.
 */
class Mesh : IResource
{
    friend class Graphics;

private:
    Renderer::RendererMemory m_vertexBufferData = {};
    Renderer::RendererMemory m_indexBufferData = {};

    Renderer::VertexBufferHandle m_vertexBuffer = {};
    Renderer::IndexBufferHandle m_indexBuffer = {};

    Vector3* m_vertices = nullptr;
    Vector2* m_uvs = nullptr;
    Vector3* m_normals = nullptr;
    Vector4* m_colors = nullptr;
    uint* m_indices = nullptr;

    bool m_uploaded = false;
    bool m_hasChanges = false;

    uint m_vertexSize = 0u;

    uint m_vertices_count = 0u;
    uint m_uvs_count = 0u;
    uint m_normals_count = 0u;
    uint m_colors_count = 0u;
    uint m_indices_count = 0u;

public:
IRESOURCE_IMPL(Mesh)

private:
    void Init();

public:
    /**
	 * \brief Set vertices for this mesh.
	 * \param vertices Vertices pointer.
	 * \param count The count of vertices.
	 * 
	 * \note To apply changes, you must call 'applyChanges'
	 * after setting all data you need.
	 */
    void SetVertices(Vector3* vertices, uint count);

    /**
    * \brief Set uvs for this mesh.
    * \param uvs pointer.
    *
    * \note To apply changes, you must call 'applyChanges'
    * after setting all data you need.
    */
    void SetUVs(Vector2* uvs);

    /**
    * \brief Set normals for this mesh.
    * \param normals pointer.
    *
    * \note To apply changes, you must call 'applyChanges'
    * after setting all data you need.
    */
    void SetNormals(Vector3* normals);

    /**
    * \brief Set colors for this mesh.
    * \param colors pointer.
    *
    * \note To apply changes, you must call 'applyChanges'
    * after setting all data you need.
    */
    void SetColors(Vector4* colors);

    /**
    * \brief Set colors for this mesh.
    * \param indices pointer.
	 * \param count The count of indices.
    *
    * \note To apply changes, you must call 'applyChanges'
    * after setting all data you need.
    */
    void SetIndices(uint* indices, uint count);

    /**
	 * \brief Indicates the upload state of this mesh.
	 * \return True when all changes are uploaded to the GPU.
	 */
    bool IsUploaded() const;

    /**
	 * \brief Can the mesh be uploaded?
	 * \return True when it can be.
	 */
    bool CanUpload();

    /**
	 * \brief Apply changes.
	 */
    void ApplyChanges();

    /**
	 * \brief Upload buffers to the GPU.
	 */
    void Upload();

    /**
	 * \brief Disposes this mesh.
	 */
    void Dispose() override;

public:
    int GetVertexCount() const
    {
        return m_vertices_count;
    }

public:
    /**
	 * \brief Creates new mesh.
	 * \return The newly created mesh pointer.
	 */
    static RefPtr<Mesh> CreateMesh();
};

#endif // MESH_H
