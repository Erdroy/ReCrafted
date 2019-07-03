// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/List.h"
#include "Common/Array.h"
#include "Rendering/Renderer/Renderer.h"
#include "Scripting/Object.h"

API_USING("ReCrafted.API.Mathematics")

/// <summary>
///     Mesh class.
/// </summary>
API_CLASS(public, sealed)
class Mesh final : public Object
{
    friend class RenderingManager;
    API_CLASS_BODY()

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

private:
    void UploadAsync();
    void UploadSync();
    void ReleaseData();

private:
    Mesh() = default;

public:
    ~Mesh();

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
    /// <param name="vertices">The vertices array.</param>
    /// <remarks>To apply changes, you must call 'ApplyChanges' after setting all data you need.</remarks>
    API_FUNCTION()
    void SetVertices(const Array<Vector3>& vertices);

    /// <summary>
    ///     Set uvs for this mesh.
    /// </summary>
    /// <param name="uvs">uvs pointer.</param>
    /// <remarks>To apply changes, you must call 'ApplyChanges' after setting all data you need.</remarks>
    /// <remarks>The count of uvs must be the same as the vertex count!</remarks>
    API_FUNCTION()
    void SetUVs(const Array<Vector2>& uvs);

    /// <summary>
    ///     Set normals for this mesh.
    /// </summary>
    /// <param name="normals">normals pointer.</param>
    /// <remarks>To apply changes, you must call 'ApplyChanges' after setting all data you need.</remarks>
    API_FUNCTION()
    void SetNormals(const Array<Vector3>& normals);

    /// <summary>
    ///     Set colors for this mesh.
    /// </summary>
    /// <param name="colors">colors pointer.</param>
    /// <remarks>To apply changes, you must call 'ApplyChanges' after setting all data you need.</remarks>
    API_FUNCTION()
    void SetColors(const Array<Vector4>& colors);

    /// <summary>
    ///     Adds custom data to the mesh. Should contain the same amount of elements as vertices.
    /// </summary>
    /// <param name="ptr">Custom data pointer.</param>
    /// <param name="customStride">The custom data pointer type size / data stride.</param>
    void AddCustomData(void* ptr, size_t customStride);

    /// <summary>
    ///     Set indices for this mesh.
    /// </summary>
    /// <param name="indices">The array of indices.</param>
    /// <remarks>To apply changes, you must call 'ApplyChanges' after setting all data you need.</remarks>
    API_FUNCTION()
    void SetIndices(const Array<uint>& indices);

    /// <summary>
    ///     Applies all changes.
    /// </summary>
    /// <param name="releaseMeshData">
    ///     When true, this function will release the mesh data (vertices, colors, indices etc.) 
    ///     and all GetVertices/GetNormals will become not available.
    /// </param>
    /// <param name="autoUpload">Should this mesh be uploaded after all changes are applied?</param>
    /// <param name="asyncUpload">Should this mesh be uploaded using async function (i.e. dispatched to the render thread)?</param>
    /// <remarks>When mesh is being uploaded using async, it will be available in the NEXT FRAME for rendering.</remarks>
    API_FUNCTION()
    void ApplyChanges(bool releaseMeshData = true, bool autoUpload = true, bool asyncUpload = true);

    /// <summary>
    ///     Upload buffers to the GPU.
    /// </summary>
    /// <param name="asyncUpload">Should mesh be uploaded using async function (i.e. dispatched to the render thread)?</param>
    /// <remarks>When mesh is being uploaded using async, it will be available in the NEXT FRAME for rendering.</remarks>
    API_FUNCTION()
    void Upload(bool asyncUpload = true);

public:
    /// <summary>
    ///     Gets vertices array from this mesh.
    /// </summary>
    /// <remarks>Available only before calling ApplyChanges with releaseMeshData parameter as true.</remarks>
    API_PROPERTY(noprefix)
    Array<Vector3> Vertices() const
    {
        return { m_vertices, m_vertices_count };
    }

    /// <summary>
    ///     Gets uvs array from this mesh.
    /// </summary>
    /// <remarks>Available only before calling ApplyChanges with releaseMeshData parameter as true.</remarks>
    API_PROPERTY(noprefix)
    Array<Vector2> UVs() const
    {
        return { m_uvs, m_vertices_count };
    }

    /// <summary>
    ///     Gets normals array from this mesh.
    /// </summary>
    /// <remarks>Available only before calling ApplyChanges with releaseMeshData parameter as true.</remarks>
    API_PROPERTY(noprefix)
    Array<Vector3> Normals() const
    {
        return { m_normals, m_vertices_count };
    }

    /// <summary>
    ///     Gets colors array from this mesh.
    /// </summary>
    /// <remarks>Available only before calling ApplyChanges with releaseMeshData parameter as true.</remarks>
    API_PROPERTY(noprefix)
    Array<Vector4> Colors() const
    {
        return { m_colors, m_vertices_count };
    }

    /// <summary>
    ///     Gets indices from this mesh.
    /// </summary>
    /// <remarks>Available only before calling ApplyChanges with releaseMeshData parameter as true.</remarks>
    API_PROPERTY(noprefix)
    Array<uint> Indices() const
    {
        return { m_indices, m_indices_count };
    }

    /// <summary>
    ///     Returns the amount of vertices (and of course the count of uvs, colors and normals, as it is the same).
    /// </summary>
    API_PROPERTY(noprefix)
    int VertexCount() const
    {
        return static_cast<int>(m_vertices_count);
    }

    /// <summary>
    ///     Returns the amount of indices.
    /// </summary>
    API_PROPERTY(noprefix)
    int IndexCount() const
    {
        return static_cast<int>(m_indices_count);
    }

    /// <summary>
    ///     Indicates the upload state of this mesh.
    /// </summary>
    /// <returns>True when all changes are uploaded to the GPU.</returns>
    API_PROPERTY()
    bool IsUploaded() const;

    /// <summary>
    ///     Can the mesh be uploaded?
    /// </summary>
    /// <returns>True when it can be.</returns>
    API_PROPERTY()
    bool CanUpload() const;

public:
    /// <summary>
    ///     Creates new mesh.
    /// </summary>
    /// <returns>The newly created mesh pointer.</returns>
    API_FUNCTION()
    static Mesh* CreateMesh();
};
