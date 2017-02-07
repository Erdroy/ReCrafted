// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Mesh.h"
#include "../Core/GameCore.h"

void Mesh::init()
{

}

void Mesh::setVertices(vector3f* vertices, uint count)
{
	m_vertices = vertices;
	m_vertices_count = count;
}

void Mesh::setUVs(vector2f* uvs, uint count)
{
	m_uvs = uvs;
	m_uvs_count = count;
}

void Mesh::setNormals(vector3f* normals, uint count)
{
	m_normals = normals;
	m_normals_count = count;
}

void Mesh::setColors(vector4f* colors, uint count)
{
	m_colors = colors;
	m_colors_count = count;
}

void Mesh::setIndices(uint* indices, uint count)
{
	m_indices = indices;
	m_indices_count = count;
}

void Mesh::applyChanges()
{
	_ASSERT(m_vertices);
	_ASSERT(m_vertices_count != 0);

	if(m_vertexBuffer.idx != 0)
	{
		// TODO: free
	}

	if(m_indexBuffer.idx != 0)
	{
		// TODO: free
	}

	m_vertexdecl = {};
	m_vertexdecl.begin(bgfx::getRendererType());
	m_vertexdecl.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);

	if (m_uvs) 
		m_vertexdecl.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float);

	if (m_normals) 
		m_vertexdecl.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float);

	if (m_colors) 
		m_vertexdecl.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float);

	m_vertexdecl.end();

	// allocate memory for vertex buffer
	auto vertexMemory = bgfx::alloc(m_vertices_count * m_vertexdecl.getStride());
	auto memoryPtr = vertexMemory->data;

	// build mesh data
	for(auto i = 0u; i < m_vertices_count; i ++)
	{
		auto offset = i * m_vertexdecl.getStride();
		auto dataOffset = 0u;

		auto vertice = m_vertices[i];
		memcpy(memoryPtr + offset + dataOffset, &vertice, vertice.size());
		dataOffset += 3 * sizeof(float);

		if(m_uvs)
		{
			auto uv = m_uvs[i];
			memcpy(memoryPtr + offset + dataOffset, &uv, uv.size());
			dataOffset += 2 * sizeof(float);
		}
		if (m_normals)
		{
			auto normal = m_normals[i];
			memcpy(memoryPtr + offset + dataOffset, &normal, normal.size());
			dataOffset += 3 * sizeof(float);
		}
		if (m_colors)
		{
			auto color = m_colors[i];
			memcpy(memoryPtr + offset + dataOffset, &color, color.size());
		}
	}

	// allocate memory for index buffer
	auto indexMemory = bgfx::alloc(m_indices_count);
	memoryPtr = vertexMemory->data;

	for(auto i = 0u; i < m_indices_count; i ++)
	{
		auto indice = m_indices[i];
		auto offset = i * sizeof(uint);
		memcpy(memoryPtr + offset, &indice, sizeof(uint));
	}

	m_vertexBuffer = bgfx::createVertexBuffer(vertexMemory, m_vertexdecl);
	m_indexBuffer = bgfx::createIndexBuffer(indexMemory);

	m_vertices = nullptr;
	m_uvs = nullptr;
	m_normals = nullptr;
	m_colors = nullptr;
	m_indices = nullptr;
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
		// TODO: free
	}

	if (m_indexBuffer.idx != 0)
	{
		// TODO: free
	}
	// do not commit suicide, this type of object is handled in soft pointers usually(and SHOULD BE!)
}

Ptr<Mesh> Mesh::createMesh()
{
	Ptr<Mesh> mesh(new Mesh);
	mesh->init();

	return mesh;
}