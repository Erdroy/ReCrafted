// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Mesh.h"

void Mesh::UploadAsync()
{
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

void Mesh::UploadSync()
{
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

void Mesh::ReleaseData()
{
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
}

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

    m_customData.Clear();

    // Release mesh data
    ReleaseData();

    m_vertices_count = 0u;
    m_uvs_count = 0u;
    m_normals_count = 0u;
    m_colors_count = 0u;
    m_indices_count = 0u;
}

void Mesh::SetVertices(const Array<Vector3>& vertices)
{
    // Setup and allocate data
    m_vertices = new Vector3[vertices.Count()];
    m_vertices_count = static_cast<uint>(vertices.Count());

    // Copy data
    memcpy(static_cast<void*>(m_vertices), static_cast<const void*>(vertices.Data()), vertices.Count() * sizeof(Vector3));
}

void Mesh::SetUVs(const Array<Vector2>& uvs)
{
    _ASSERT_(m_vertices_count > 0, "Please, set vertices at first");
    ASSERT(uvs.Count() == m_vertices_count);

    // Setup and allocate data
    m_uvs = new Vector2[m_vertices_count];
    m_uvs_count = m_vertices_count;

    // Copy data
    memcpy(static_cast<void*>(m_uvs), static_cast<const void*>(uvs.Data()), m_vertices_count * sizeof(Vector2));
}

void Mesh::SetNormals(const Array<Vector3>& normals)
{
    _ASSERT_(m_vertices_count > 0, "Please, set vertices at first");
    ASSERT(normals.Count() == m_vertices_count);

    // Setup and allocate data
    m_normals = new Vector3[m_vertices_count];
    m_normals_count = static_cast<uint>(m_vertices_count);

    // Copy data
    memcpy(static_cast<void*>(m_normals), static_cast<const void*>(normals.Data()), m_vertices_count * sizeof(Vector3));
}

void Mesh::SetColors(const Array<Vector4>& colors)
{
    _ASSERT_(m_vertices_count > 0, "Please, set vertices at first");
    ASSERT(colors.Count() == m_vertices_count);

    // Setup and allocate data
    m_colors = new Vector4[m_vertices_count];
    m_colors_count = static_cast<uint>(m_vertices_count);

    // Copy data
    memcpy(static_cast<void*>(m_colors), static_cast<const void*>(colors.Data()), m_vertices_count * sizeof(Vector4));
}

void Mesh::AddCustomData(void* ptr, const size_t customStride)
{
    // TODO: Validate custom data size

    m_customData.Add(CustomDataType{ customStride, static_cast<uint8_t*>(ptr) });
}

void Mesh::SetIndices(const Array<uint>& indices)
{
    ASSERT(indices.Count() > 0);
    _ASSERT_(indices.Count() % 3 == 0, "Only triangles are supported.");

    // Setup and allocate data
    m_indices = new uint[indices.Count()];
    m_indices_count = static_cast<uint>(indices.Count());

    // Copy data
    memcpy(static_cast<void*>(m_indices), indices.Data(), indices.Count() * sizeof(uint));
}

bool Mesh::IsUploaded() const
{
    return m_uploaded;
}

bool Mesh::CanUpload() const
{
    return m_hasChanges;
}

void Mesh::ApplyChanges(const bool releaseMeshData, const bool autoUpload, const bool asyncUpload)
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
            m_vertexSize += static_cast<uint>(dataType.dataWidth);
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
        auto vertex = m_vertices[i];
        memcpy(memoryPtr + offset + dataOffset, static_cast<const void*>(&vertex), sizeof(float) * 3);
        dataOffset += 3 * sizeof(float);

        // Copy normals
        if (m_normals)
        {
            memcpy(memoryPtr + offset + dataOffset, static_cast<const void*>(&m_normals[i]), sizeof(float) * 3);
            dataOffset += 3 * sizeof(float);
        }

        // Copy uvs
        if (m_uvs)
        {
            memcpy(memoryPtr + offset + dataOffset, static_cast<const void*>(&m_uvs[i]), sizeof(float) * 2);
            dataOffset += 2 * sizeof(float);
        }

        // Copy colors
        if (m_colors)
        {
            memcpy(memoryPtr + offset + dataOffset, static_cast<const void*>(&m_colors[i]), sizeof(float) * 4);
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
                dataOffset += static_cast<uint>(type.dataWidth);
            }
        }
    }

    // allocate memory for index buffer
    m_indexBufferData = Renderer::Allocate(m_indices_count * sizeof(uint), 0);
    memoryPtr = reinterpret_cast<byte*>(m_indexBufferData);

    for (auto i = 0u; i < m_indices_count; i++)
    {
        auto index = m_indices[i];
        const auto offset = i * sizeof(uint);
        memcpy(memoryPtr + offset, &index, sizeof(uint));
    }

    m_uploaded = false;
    m_hasChanges = true;

    m_customData.Clear();

    if(releaseMeshData)
    {
        ReleaseData();
    }

    if(autoUpload)
    {
        Upload(asyncUpload);
    }
}

void Mesh::Upload(const bool asyncUpload)
{
    ASSERT(m_hasChanges);
    ASSERT(m_uploaded == false);

    ASSERT(m_vertexBufferData != nullptr);
    ASSERT(m_indexBufferData != nullptr);

    if(asyncUpload)
    {
        // Upload this mesh using async method.
        UploadAsync();
    }
    else
    {
        // Upload this mesh using sync (immediate) method.
        UploadSync();
    }
}

Mesh* Mesh::CreateMesh()
{
    return New<Mesh>();
}
