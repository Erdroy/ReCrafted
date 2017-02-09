// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Rendering.h"

Rendering* Rendering::m_instance;


void Rendering::init()
{
	// load all shaders

	m_testMesh = Mesh::createMesh();

	vector3f vertices[] = {
		vector3f(0.0f, 0.0f, 0.0f),
		vector3f(0.0f, 1.0f, 0.0f),
		vector3f(1.0f, 0.0f, 0.0f),
	};
	m_testMesh->setVertices(vertices, 3);

	uint indices[] = {
		0, 1, 2
	};
	m_testMesh->setIndices(indices, 3);

	m_testMesh->applyChanges();

	m_testShader = Shader::loadShader("testShader");
}

void Rendering::beginRender()
{
}

void Rendering::endRender()
{
	
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

void Rendering::draw(Ptr<Mesh> mesh)
{

}

void Rendering::dispose()
{


	// suicide
	delete this;
}
