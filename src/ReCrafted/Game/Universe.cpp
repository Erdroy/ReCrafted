// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Universe.h"
#include "../Graphics/Rendering.h"

Universe* Universe::m_instance;

void Universe::init()
{
	// create test mesh
	Vector3 vertices[7] = {
		Vector3(-1.0f, -1.0f, 0.0f),
		Vector3( 0.0f,  1.0f, 0.0f),
		Vector3( 1.0f, -1.0f, 0.0f),

		Vector3( 10.0f, -5.0f,  10.0f),
		Vector3(-10.0f, -5.0f,  10.0f),
		Vector3(-10.0f, -5.0f, -10.0f),
		Vector3( 10.0f, -5.0f, -10.0f),
	};

	Vector2 uvs[7] = {
		Vector2(0.0f, 0.0f),
		Vector2(1.0f, 0.0f),
		Vector2(1.0f, 1.0f),

		Vector2(0.0f, 0.0f),
		Vector2(1.0f, 0.0f),
		Vector2(1.0f, 1.0f),
		Vector2(0.0f, 1.0f),
	};

	Vector3 normals[7] = {
		Vector3(0.0f, 0.0f, -1.0f),
		Vector3(0.0f, 0.0f, -1.0f),
		Vector3(0.0f, 0.0f, -1.0f),

		Vector3(0.0f, 1.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f),
	};

	uint indices[9] = {
		2, 1, 0,

		3, 4, 5,
		3, 5, 6
	};

	m_testMesh = Mesh::createMesh();
	m_testMesh->setVertices(vertices, 7);
	m_testMesh->setUVs(uvs);
	m_testMesh->setNormals(normals);
	m_testMesh->setIndices(indices, 9);
	m_testMesh->applyChanges();
}

void Universe::update()
{

}

void Universe::simulate()
{

}

void Universe::drawShadowCasters()
{

}

void Universe::draw()
{
	auto modelMatrix = Matrix::identity();
	Rendering::getInstance()->draw(m_testMesh, &modelMatrix);
}
