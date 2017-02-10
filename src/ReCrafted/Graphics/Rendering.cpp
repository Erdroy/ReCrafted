// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Rendering.h"

Rendering* Rendering::m_instance;


void Rendering::init()
{
	// load all shaders

	m_testMesh = Mesh::createMesh();

	vector3f vertices[] = {
		vector3f(-0.5f, -0.5f, 0.0f),
		vector3f(-0.5f,  0.5f, 0.0f),
		vector3f( 0.5f,  0.5f, 0.0f),
		vector3f( 0.5f, -0.5f, 0.0f),
	};
	m_testMesh->setVertices(vertices, 4);

	uint indices[] = {
		2, 1, 0,
		0, 3, 2
	};
	m_testMesh->setIndices(indices, 6);

	m_testMesh->applyChanges();

	m_testShader = Shader::loadShader("testShader");
}

void Rendering::beginRender()
{
}

void Rendering::endRender()
{
	draw(m_testMesh, m_testShader);
}

void Rendering::renderShadows()
{
}

void Rendering::renderStatic()
{
}

void Rendering::renderEntities()
{
}

void Rendering::draw(Ptr<Mesh> mesh, Ptr<Shader> shader)
{
	setVertexBuffer(mesh->m_vertexBuffer);
	setIndexBuffer(mesh->m_indexBuffer);
	submit(0, shader->m_program);
}

void Rendering::dispose()
{


	// suicide
	delete this;
}
