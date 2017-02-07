// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Mesh.h"

void Mesh::init()
{
}

void Mesh::dispose()
{
	// clean
	m_vertexCount = 0u;
	m_indexCount = 0u;

	// do not commit suicide, this type of object is handled in soft pointers usually(and SHOULD BE!)
}

Ptr<Mesh> Mesh::createMesh()
{
	Ptr<Mesh> mesh(new Mesh);
	mesh->init();

	return mesh;
}