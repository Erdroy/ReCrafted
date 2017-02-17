// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Rendering.h"
#include "Camera.h"

Rendering* Rendering::m_instance;

bgfx::UniformHandle _wvp;

void Rendering::init()
{
	// load all shaders
	m_testMesh = Mesh::createMesh();

	Vector3 vertices[] = {
		Vector3(-1.0f, -1.0f, 0.0f),
		Vector3( 0.0f,  1.0f, 0.0f),
		Vector3( 1.0f, -1.0f, 0.0f),

		Vector3(-5.0f, -2.0f, -5.0f),
		Vector3(-5.0f, -2.0f,  5.0f),
		Vector3( 5.0f, -2.0f,  5.0f),
		Vector3( 5.0f, -2.0f, -5.0f),
	};
	m_testMesh->setVertices(vertices, 7);

	uint indices[] = {
		2, 1, 0,

		5, 4, 3,
		3, 6, 5
	};
	m_testMesh->setIndices(indices, 9);

	m_testMesh->applyChanges();

	_wvp = bgfx::createUniform("WVP", bgfx::UniformType::Mat4);

	m_testShader = Shader::loadShader("testShader");
}

void Rendering::resize(uint width, uint height)
{
	if (Camera::m_mainCamera == nullptr)
	{
		VS_LOG("WARNING: Main camera is not set to.");
		return;
	}

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
	auto model = Matrix::identity();

	draw(m_testMesh, m_testShader, &model);
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

void Rendering::draw(Ptr<Mesh> mesh, Ptr<Shader> shader, Matrix* modelMatrix)
{
	setVertexBuffer(mesh->m_vertexBuffer);
	setIndexBuffer(mesh->m_indexBuffer);

	auto view = Camera::m_mainCamera->m_view;
	auto proj = Camera::m_mainCamera->m_projection;

	auto mat = *modelMatrix * view * proj;
	mat.transpose();

	bgfx::setUniform(_wvp, &mat);

	submit(0, shader->m_program);
}

void Rendering::dispose()
{
	bgfx::destroyUniform(_wvp);

	// suicide
	delete this;
}
