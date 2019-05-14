// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Mesh.h"

Mesh::~Mesh()
{
    if (RENDERER_CHECK_HANDLE(m_vertexBuffer))
        Renderer::DestroyVertexBuffer(m_vertexBuffer);

    if (RENDERER_CHECK_HANDLE(m_indexBuffer))
        Renderer::DestroyIndexBuffer(m_indexBuffer);

    if (m_vertexBufferData)
        Renderer::Free(m_vertexBufferData);

    if (m_indexBufferData)
        Renderer::Free(m_indexBufferData);

    // delete data
    delete[] m_vertices;
    delete[] m_uvs;
    delete[] m_normals;
    delete[] m_colors;
    delete[] m_indices;

    // clean
    m_vertices = nullptr;
    m_uvs = nullptr;
    m_normals = nullptr;
    m_colors = nullptr;
    m_indices = nullptr;

    m_customData.Clear();

    m_vertices_count = 0u;
    m_uvs_count = 0u;
    m_normals_count = 0u;
    m_colors_count = 0u;
    m_indices_count = 0u;
}

void Mesh::Init()
{
}

void Mesh::SetVertices(Vector3* vertices, uint count)
{
    // Setup and allocate data
    m_vertices = new Vector3[count];
    m_vertices_count = count;

    // Copy data
    memcpy(static_cast<void*>(m_vertices), reinterpret_cast<uint8_t*>(vertices), count * sizeof(Vector3));
}

void Mesh::SetUVs(Vector2* uvs)
{
    // Setup and allocate data
    m_uvs = new Vector2[m_vertices_count];
    m_uvs_count = m_vertices_count;

    // Copy data
    memcpy(static_cast<void*>(m_uvs), reinterpret_cast<uint8_t*>(uvs), m_vertices_count * sizeof(Vector2));
}

void Mesh::SetNormals(Vector3* normals)
{
    // Setup and allocate data
    m_normals = new Vector3[m_vertices_count];
    m_normals_count = m_vertices_count;

    // Copy data
    memcpy(static_cast<void*>(m_normals), reinterpret_cast<uint8_t*>(normals), m_vertices_count * sizeof(Vector3));
}

void Mesh::SetColors(Vector4* colors)
{
    // Setup and allocate data
    m_colors = new Vector4[m_vertices_count];
    m_colors_count = m_vertices_count;

    // Copy data
    memcpy(static_cast<void*>(m_colors), reinterpret_cast<uint8_t*>(colors), m_vertices_count * sizeof(Vector4));
}

void Mesh::AddCustomData(void* ptr, const size_t customStride)
{
    m_customData.Add(CustomDataType{ customStride, static_cast<uint8_t*>(ptr) });
}

void Mesh::SetIndices(uint* indices, uint count)
{
    // Setup and allocate data
    m_indices = new uint[m_vertices_count];
    m_indices_count = count;

    // Copy data
    memcpy(static_cast<void*>(m_indices), indices, count * sizeof(uint));
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

    if (m_customData.Count() > 0)
    {
        for (auto& dataType : m_customData)
            m_vertexSize += dataType.dataWidth;
    }

    // allocate memory for vertex buffer
    m_vertexBufferData = Renderer::Allocate(m_vertices_count * m_vertexSize, 0);
    auto memoryPtr = reinterpret_cast<byte*>(m_vertexBufferData);

    // build mesh data
    for (auto i = 0u; i < m_vertices_count; i++)
    {
        const auto offset = i * m_vertexSize;
        auto dataOffset = 0u;

        // Copy positions
        auto vertice = m_vertices[i];
        memcpy(memoryPtr + offset + dataOffset, &vertice, sizeof(float) * 3);
        dataOffset += 3 * sizeof(float);

        // Copy normals
        if (m_normals)
        {
            memcpy(memoryPtr + offset + dataOffset, &m_normals[i], sizeof(float) * 3);
            dataOffset += 3 * sizeof(float);
        }

        // Copy uvs
        if (m_uvs)
        {
            memcpy(memoryPtr + offset + dataOffset, &m_uvs[i], sizeof(float) * 2);
            dataOffset += 2 * sizeof(float);
        }

        // Copy colors
        if (m_colors)
        {
            memcpy(memoryPtr + offset + dataOffset, &m_colors[i], sizeof(float) * 4);
            dataOffset += 4 * sizeof(float);
        }

        // Copy custom data
        if (m_customData.Count() > 0)
        {
            for (auto& type : m_customData)
            {
                memcpy(memoryPtr + offset + dataOffset, type.dataPtr, type.dataWidth);

                // Add offset to the data pointer.
                // This allows us to use the dataPtr pointer as an array, like every other default type.
                type.dataPtr += type.dataWidth;
                dataOffset += type.dataWidth;
            }
        }
    }

    // allocate memory for index buffer
    m_indexBufferData = Renderer::Allocate(m_indices_count * sizeof(uint), 0);
    memoryPtr = reinterpret_cast<byte*>(m_indexBufferData);

    for (auto i = 0u; i < m_indices_count; i++)
    {
        auto indice = m_indices[i];
        const auto offset = i * sizeof(uint);
        memcpy(memoryPtr + offset, &indice, sizeof(uint));
    }

    m_uploaded = false;
    m_hasChanges = true;

    m_customData.Clear();
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

RefPtr<Mesh> Mesh::CreateMesh()
{
    RefPtr<Mesh> mesh(new Mesh);
    mesh->Init();
    return mesh;
}
