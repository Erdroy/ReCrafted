// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Rendering.h"
#include "Camera.h"

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

void Rendering::resize(uint width, uint height)
{
	// update main camera perspective
	Camera::m_mainCamera->updatePerspective();
}

void Rendering::beginRender()
{
	if(Camera::m_mainCamera == nullptr)
	{
		VS_LOG("WARNING: Trying to render scene without any camera set as main!");
		return;
	}

	// update main camera
	Camera::m_mainCamera->update();
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

void Rendering::draw(Ptr<Mesh> mesh, Ptr<Shader> shader, matrix44f modelMatrix)
{
	setVertexBuffer(mesh->m_vertexBuffer);
	setIndexBuffer(mesh->m_indexBuffer);

	bgfx::setTransform(modelMatrix.data());

	submit(0, shader->m_program);
}

void Rendering::dispose()
{


	// suicide
	delete this;
}
