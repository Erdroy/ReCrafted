// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef MESH_H
#define MESH_H

// includes
#include "../Utils/Types.h"

class Mesh
{
private:
	uint m_vertexCount = 0u;
	uint m_indexCount = 0u;

private:
	void init();

	void setVertices();

	void setColors();

	void setNormals();

	void setIndices();

	void applyChanges();

public:
	void dispose();

public:
	static Ptr<Mesh> createMesh();

};

#endif // MESH_H