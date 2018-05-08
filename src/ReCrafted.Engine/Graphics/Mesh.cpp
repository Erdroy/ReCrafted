// ReCrafted (c) 2016-2018 Always Too Late

#include "Mesh.h"
#include "Core/Math/MeshSimplification/ng_mesh_simplify.h"
#include "Renderer/Renderer.hpp"

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

    m_vertexSize = sizeof(Vector3);
    if (m_uvs)
        m_vertexSize += sizeof(Vector2);

	if (m_normals) 
        m_vertexSize += sizeof(Vector3);

	if (m_colors)
        m_vertexSize += sizeof(Vector4);

	// allocate memory for vertex buffer
	m_vertexBufferData = Renderer::Allocate(m_vertices_count * m_vertexSize);
	var memoryPtr = reinterpret_cast<byte*>(m_vertexBufferData);

	// build mesh data
	for(auto i = 0u; i < m_vertices_count; i ++)
	{
		cvar offset = i * m_vertexSize;
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
	m_indexBufferData = Renderer::Allocate(m_indices_count * sizeof(uint));
	memoryPtr = reinterpret_cast<byte*>(m_indexBufferData);

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

void Mesh::simplify()
{
	cvar simplfiedMesh = new MeshBuffer;
	simplfiedMesh->numVertices = m_vertices_count;
	simplfiedMesh->vertices = new MeshVertex[m_vertices_count];

	for (var i = 0u; i < m_vertices_count; i++)
	{
		simplfiedMesh->vertices[i].xyz[0] = m_vertices[i].x;
		simplfiedMesh->vertices[i].xyz[1] = m_vertices[i].y;
		simplfiedMesh->vertices[i].xyz[2] = m_vertices[i].z;
		simplfiedMesh->vertices[i].xyz[3] = 0.0f;

		simplfiedMesh->vertices[i].normal[0] = m_normals[i].x;
		simplfiedMesh->vertices[i].normal[1] = m_normals[i].y;
		simplfiedMesh->vertices[i].normal[2] = m_normals[i].z;
		simplfiedMesh->vertices[i].normal[3] = 0.0f;

		simplfiedMesh->vertices[i].colour[0] = m_colors[i].x;
		simplfiedMesh->vertices[i].colour[1] = m_colors[i].y;
		simplfiedMesh->vertices[i].colour[2] = m_colors[i].z;
		simplfiedMesh->vertices[i].colour[3] = m_colors[i].w;
	}

	simplfiedMesh->numTriangles = m_indices_count / 3;
    simplfiedMesh->triangles = new MeshTriangle[m_indices_count / 3];

	memcpy(simplfiedMesh->triangles, m_indices, sizeof(MeshTriangle) * (m_indices_count / 3));

	vec4 offset;
	offset[0] = 1.0f;
	offset[1] = 1.0f;
	offset[2] = 1.0f;
	offset[3] = 1.0f;

	MeshSimplificationOptions options;
	options.edgeFraction = 0.125f;
	options.maxIterations = 10;
	options.targetPercentage = 0.05f;
	options.maxError = 1.0f;
	options.maxEdgeSize = 0.5f;
	options.minAngleCosine = 0.8f;

	ngMeshSimplifier(simplfiedMesh, offset, options);

	m_vertices_count = simplfiedMesh->numVertices;
	m_normals_count = m_vertices_count;
	m_colors_count = m_vertices_count;
	m_indices_count = simplfiedMesh->numTriangles * 3;

	for (var i = 0; i < simplfiedMesh->numVertices; i++)
	{
		m_vertices[i].x = simplfiedMesh->vertices[i].xyz[0];
		m_vertices[i].y = simplfiedMesh->vertices[i].xyz[1];
		m_vertices[i].z = simplfiedMesh->vertices[i].xyz[2];

		m_normals[i].x = simplfiedMesh->vertices[i].normal[0];
		m_normals[i].y = simplfiedMesh->vertices[i].normal[1];
		m_normals[i].z = simplfiedMesh->vertices[i].normal[2];

		m_colors[i].x = simplfiedMesh->vertices[i].colour[0];
		m_colors[i].y = simplfiedMesh->vertices[i].colour[1];
		m_colors[i].z = simplfiedMesh->vertices[i].colour[2];
		m_colors[i].w = simplfiedMesh->vertices[i].colour[3];
	}

    memcpy(m_indices, simplfiedMesh->triangles, sizeof(MeshTriangle) * simplfiedMesh->numTriangles);

    delete[] simplfiedMesh->vertices;
	delete[] simplfiedMesh->triangles;

	delete simplfiedMesh;
}

void Mesh::upload()
{
	if (m_vertexBufferData == nullptr || m_indexBufferData == nullptr)
		return;

    if (RENDERER_CHECK_HANDLE(m_vertexBuffer))
        Renderer::DestroyVertexBuffer(m_vertexBuffer);

    if (RENDERER_CHECK_HANDLE(m_indexBuffer))
        Renderer::DestroyIndexBuffer(m_indexBuffer);

	m_vertexBuffer = Renderer::CreateVertexBuffer(m_vertices_count, m_vertexSize, m_vertexBufferData);
	m_indexBuffer = Renderer::CreateIndexBuffer(m_indices_count, m_indexBufferData);

    m_vertexBufferData = nullptr;
    m_indexBufferData = nullptr;

	m_uploaded = true;
	m_hasChanges = false;
}

void Mesh::dispose()
{
    if (RENDERER_CHECK_HANDLE(m_vertexBuffer))
        Renderer::DestroyVertexBuffer(m_vertexBuffer);

    if (RENDERER_CHECK_HANDLE(m_indexBuffer))
        Renderer::DestroyIndexBuffer(m_indexBuffer);

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

Ref<Mesh> Mesh::createMesh()
{
	Ref<Mesh> mesh(new Mesh);
	mesh->init();
	return mesh;
}