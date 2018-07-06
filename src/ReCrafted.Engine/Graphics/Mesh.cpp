// ReCrafted (c) 2016-2018 Always Too Late

#include "Mesh.h"
#include "Renderer/Renderer.hpp"

void Mesh::Init()
{
}

void Mesh::SetVertices(Vector3* vertices, uint count)
{
    m_vertices = vertices;
    m_vertices_count = count;
}

void Mesh::SetUVs(Vector2* uvs)
{
    m_uvs = uvs;
    m_uvs_count = m_vertices_count;
}

void Mesh::SetNormals(Vector3* normals)
{
    m_normals = normals;
    m_normals_count = m_vertices_count;
}

void Mesh::SetColors(Vector4* colors)
{
    m_colors = colors;
    m_colors_count = m_vertices_count;
}

void Mesh::SetIndices(uint* indices, uint count)
{
    m_indices = indices;
    m_indices_count = count;
}

bool Mesh::IsUploaded() const
{
    return m_uploaded;
}

bool Mesh::CanUpload()
{
    return m_hasChanges;
}

void Mesh::ApplyChanges()
{
    ASSERT(m_vertices);
    ASSERT(m_vertices_count > 0);
    ASSERT(m_indices_count > 0);

    ASSERT(m_vertexBufferData == nullptr);
    ASSERT(m_indexBufferData == nullptr);

    m_vertexSize = sizeof(Vector3);
    if (m_uvs)
        m_vertexSize += sizeof(Vector2);

    if (m_normals)
        m_vertexSize += sizeof(Vector3);

    if (m_colors)
        m_vertexSize += sizeof(Vector4);

    // allocate memory for vertex buffer
    m_vertexBufferData = Renderer::Allocate(m_vertices_count * m_vertexSize, 0);
    var memoryPtr = reinterpret_cast<byte*>(m_vertexBufferData);

    // build mesh data
    for (auto i = 0u; i < m_vertices_count; i ++)
    {
        cvar offset = i * m_vertexSize;
        var dataOffset = 0u;

        var vertice = m_vertices[i];

        memcpy(memoryPtr + offset + dataOffset, &vertice, sizeof(float) * 3);
        dataOffset += 3 * sizeof(float);

        if (m_normals)
        {
            memcpy(memoryPtr + offset + dataOffset, &m_normals[i], sizeof(float) * 3);
            dataOffset += 3 * sizeof(float);
        }
        if (m_uvs)
        {
            memcpy(memoryPtr + offset + dataOffset, &m_uvs[i], sizeof(float) * 2);
            dataOffset += 2 * sizeof(float);
        }
        if (m_colors)
        {
            memcpy(memoryPtr + offset + dataOffset, &m_colors[i], sizeof(float) * 4);
            dataOffset += 4 * sizeof(float);
        }
    }

    // allocate memory for index buffer
    m_indexBufferData = Renderer::Allocate(m_indices_count * sizeof(uint), 0);
    memoryPtr = reinterpret_cast<byte*>(m_indexBufferData);

    for (auto i = 0u; i < m_indices_count; i ++)
    {
        var indice = m_indices[i];
        cvar offset = i * sizeof(uint);
        memcpy(memoryPtr + offset, &indice, sizeof(uint));
    }

    m_uploaded = false;
    m_hasChanges = true;

    m_vertices = nullptr;
    m_uvs = nullptr;
    m_normals = nullptr;
    m_colors = nullptr;
    m_indices = nullptr;
}

void Mesh::UploadNow()
{
    ASSERT(m_vertexBufferData != nullptr);
    ASSERT(m_indexBufferData != nullptr);

    // Create vertex buffer
    m_vertexBuffer = Renderer::CreateVertexBufferSync(m_vertices_count, m_vertexSize, false, m_vertexBufferData);

    // Create index buffer
    m_indexBuffer = Renderer::CreateIndexBufferSync(m_indices_count, true, false, m_indexBufferData);

    Renderer::Free(m_vertexBufferData);
    Renderer::Free(m_indexBufferData);

    m_vertexBufferData = nullptr;
    m_indexBufferData = nullptr;

    m_uploaded = true;
    m_hasChanges = false;
}

void Mesh::Upload()
{
    ASSERT(m_vertexBufferData != nullptr);
    ASSERT(m_indexBufferData != nullptr);

    // Create vertex buffer
    m_vertexBuffer = Renderer::CreateVertexBuffer(m_vertices_count, m_vertexSize, m_vertexBufferData);

    // Create index buffer
    m_indexBuffer = Renderer::CreateIndexBuffer(m_indices_count, m_indexBufferData);

    Renderer::QueueFree(m_vertexBufferData);
    Renderer::QueueFree(m_indexBufferData);

    m_vertexBufferData = nullptr;
    m_indexBufferData = nullptr;

    m_uploaded = true;
    m_hasChanges = false;
}

void Mesh::Dispose()
{
    if (RENDERER_CHECK_HANDLE(m_vertexBuffer))
        Renderer::DestroyVertexBuffer(m_vertexBuffer);

    if (RENDERER_CHECK_HANDLE(m_indexBuffer))
        Renderer::DestroyIndexBuffer(m_indexBuffer);

    if(m_vertexBufferData)
        Renderer::Free(m_vertexBufferData);

    if (m_indexBufferData)
        Renderer::Free(m_indexBufferData);

    // clean
    m_vertices = nullptr;
    m_uvs = nullptr;
    m_normals = nullptr;
    m_colors = nullptr;
    m_indices = nullptr;

    m_vertices_count = 0u;
    m_uvs_count = 0u;
    m_normals_count = 0u;
    m_colors_count = 0u;
    m_indices_count = 0u;
}

RefPtr<Mesh> Mesh::CreateMesh()
{
    RefPtr<Mesh> mesh(new Mesh);
    mesh->Init();
    return mesh;
}
