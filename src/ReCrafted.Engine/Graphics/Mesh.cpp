// ReCrafted © 2016-2017 Always Too Late

#include "Mesh.h"

void Mesh::init()
{

}

void Mesh::setVertices(Vector3* vertices, uint count)
{
	m_vertices = vertices;
	m_vertices_count = count;
}

void Mesh::setUVs(Vector2* uvs)
{
	m_uvs = uvs;
	m_uvs_count = m_vertices_count;
}

void Mesh::setNormals(Vector3* normals)
{
	m_normals = normals;
	m_normals_count = m_vertices_count;
}

void Mesh::setColors(Vector4* colors)
{
	m_colors = colors;
	m_colors_count = m_vertices_count;
}

void Mesh::setIndices(uint* indices, uint count)
{
	m_indices = indices;
	m_indices_count = count;
}

bool Mesh::isUploaded() const
{
	return m_uploaded;
}

bool Mesh::canUpload()
{
	return m_hasChanges;
}

void Mesh::applyChanges()
{
	_ASSERT(m_vertices);
	_ASSERT(m_vertices_count != 0);
	_ASSERT(m_indices_count != 0);

	if(m_vertexBuffer.idx != 0)
	{
		// TODO: free
	}

	if(m_indexBuffer.idx != 0)
	{
		// TODO: free
	}

	m_vertexdecl = {};
	m_vertexdecl.begin();
	m_vertexdecl.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);

	if (m_uvs) 
		m_vertexdecl.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float);

	if (m_normals) 
		m_vertexdecl.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float);

	if (m_colors) 
		m_vertexdecl.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float);

	m_vertexdecl.end();

	// allocate memory for vertex buffer
	m_vertexBufferData = bgfx::alloc(m_vertices_count * m_vertexdecl.getStride());
	auto memoryPtr = m_vertexBufferData->data;

	// build mesh data
	for(auto i = 0u; i < m_vertices_count; i ++)
	{
		auto offset = i * m_vertexdecl.getStride();
		auto dataOffset = 0u;

		auto vertice = m_vertices[i];

		memcpy(memoryPtr + offset + dataOffset, &vertice, sizeof(float) * 3);
		dataOffset += 3 * sizeof(float);

		if(m_uvs)
		{
			memcpy(memoryPtr + offset + dataOffset, &m_uvs[i], sizeof(float) * 2);
			dataOffset += 2 * sizeof(float);
		}
		if (m_normals)
		{
			memcpy(memoryPtr + offset + dataOffset, &m_normals[i], sizeof(float) * 3);
			dataOffset += 3 * sizeof(float);
		}
		if (m_colors)
		{
			memcpy(memoryPtr + offset + dataOffset, &m_colors[i], sizeof(float) * 4);
			dataOffset += 4 * sizeof(float);
		}
	}

	// allocate memory for index buffer
	m_indexBufferData = bgfx::alloc(m_indices_count * sizeof(uint));
	memoryPtr = m_indexBufferData->data;

	for(auto i = 0u; i < m_indices_count; i ++)
	{
		auto indice = m_indices[i];
		auto offset = i * sizeof(uint);
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

void Mesh::upload()
{
	if (m_vertexBufferData == nullptr || m_indexBufferData == nullptr)
		return;

	m_vertexBuffer = bgfx::createVertexBuffer(m_vertexBufferData, m_vertexdecl);
	m_indexBuffer = bgfx::createIndexBuffer(m_indexBufferData, BGFX_BUFFER_INDEX32);

	m_uploaded = true;
	m_hasChanges = false;
}

void Mesh::dispose()
{
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

	if (m_vertexBuffer.idx != 0)
	{
		bgfx::destroyVertexBuffer(m_vertexBuffer);
	}

	if (m_indexBuffer.idx != 0)
	{
		bgfx::destroyIndexBuffer(m_indexBuffer);
	}
	// do not commit suicide, this type of object is handled in soft pointers usually(and SHOULD BE!)
}

Ptr<Mesh> Mesh::createMesh()
{
	Ptr<Mesh> mesh(new Mesh);
	mesh->init();

	if (mesh == nullptr)
		throw;

	return mesh;
}