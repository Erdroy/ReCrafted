// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include "Common/List.h"
#include "Rendering/Renderer/Renderer.h"

/// <summary>
///     Mesh class.
/// </summary>
class Mesh
{
private:
    struct CustomDataType
    {
        size_t dataWidth;
        uint8_t* dataPtr;
    };

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

    List<CustomDataType> m_customData;

    bool m_uploaded = false;
    bool m_hasChanges = false;

    uint m_vertexSize = 0u;

    uint m_vertices_count = 0u;
    uint m_uvs_count = 0u;
    uint m_normals_count = 0u;
    uint m_colors_count = 0u;
    uint m_indices_count = 0u;

public:
    Mesh() = default;
    ~Mesh();

private:
    void Init();

public:
    /// <summary>
    ///     Gets the vertex buffer handle.
    /// </summary>
    Renderer::VertexBufferHandle GetVertexBufferHandle() const
    {
        return m_vertexBuffer;
    }

    /// <summary>
    ///     Gets the index buffer handle.
    /// </summary>
    Renderer::IndexBufferHandle GetIndexBufferHandle() const
    {
        return m_indexBuffer;
    }

public:
    /// <summary>
    ///     Set vertices for this mesh.
    /// </summary>
    /// <param name="vertices">Vertices pointer.</param>
    /// <param name="count">The count of vertices.</param>
    /// <remarks>To apply changes, you must call 'applyChanges' after setting all data you need.</remarks>
    void SetVertices(Vector3* vertices, uint count);

    /// <summary>
    ///     Gets pointer to vertices.
    /// </summary>
    Vector3* GetVertices() const
    {
        return m_vertices;
    }

    /// <summary>
    ///     Returns the amount of vertices (and of course the count of uvs, colors and normals, as it is the same).
    /// </summary>
    size_t GetVertexCount() const
    {
        return static_cast<size_t>(m_vertices_count);
    }

    /// <summary>
    ///     Set uvs for this mesh.
    /// </summary>
    /// <param name="uvs">uvs pointer.</param>
    /// <remarks>To apply changes, you must call 'applyChanges' after setting all data you need.</remarks>
    void SetUVs(Vector2* uvs);

    /// <summary>
    ///     Gets pointer to uvs.
    /// </summary>
    Vector2* GetUVs() const
    {
        return m_uvs;
    }

    /// <summary>
    ///     Set normals for this mesh.
    /// </summary>
    /// <param name="normals">normals pointer.</param>
    /// <remarks>To apply changes, you must call 'applyChanges' after setting all data you need.</remarks>
    void SetNormals(Vector3* normals);

    /// <summary>
    ///     Gets pointer to normals.
    /// </summary>
    Vector3* GetNormals() const
    {
        return m_normals;
    }

    /// <summary>
    ///     Set colors for this mesh.
    /// </summary>
    /// <param name="colors">colors pointer.</param>
    /// <remarks>To apply changes, you must call 'applyChanges' after setting all data you need.</remarks>
    void SetColors(Vector4* colors);

    /// <summary>
    ///     Gets pointer to colors.
    /// </summary>
    Vector4* GetColors() const
    {
        return m_colors;
    }

    /// <summary>
    ///     Adds custom data to the mesh. Should contain the same amount of elements as vertices.
    /// </summary>
    /// <param name="ptr">Custom data pointer.</param>
    /// <param name="customStride">The custom data pointer type size / data stride.</param>
    void AddCustomData(void* ptr, size_t customStride);

    /// <summary>
    ///     Set indices for this mesh.
    /// </summary>
    /// <param name="indices">indices pointer.</param>
    /// <param name="count">Index count.</param>
    /// <remarks>To apply changes, you must call 'applyChanges' after setting all data you need.</remarks>
    void SetIndices(uint* indices, uint count);

    /// <summary>
    ///     Gets pointer to indices.
    /// </summary>
    uint* GetIndices() const
    {
        return m_indices;
    }

    /// <summary>
    ///     Returns the amount of indices.
    /// </summary>
    size_t GetIndexCount() const
    {
        return static_cast<size_t>(m_indices_count);
    }

    /// <summary>
    ///     Indicates the upload state of this mesh.
    /// </summary>
    /// <returns>True when all changes are uploaded to the GPU.</returns>
    bool IsUploaded() const;

    /// <summary>
    ///     Can the mesh be uploaded?
    /// </summary>
    /// <returns>True when it can be.</returns>
    bool CanUpload();

    /// <summary>
    ///     Apply changes.
    /// </summary>
    void ApplyChanges();

    /// <summary>
    ///     Upload buffers to the GPU.
    /// </summary>
    void Upload();

    /// <summary>
    ///     Upload buffers to the GPU synchronously right now.
    /// </summary>
    void UploadNow();

public:
    /// <summary>
    ///     Creates new mesh.
    /// </summary>
    /// <returns>The newly created mesh pointer.</returns>
    static RefPtr<Mesh> CreateMesh();
};
