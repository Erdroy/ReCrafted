// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef MESH_H
#define MESH_H

// includes
#include "../bgfxPrerequisites.h"
#include "../Utils/Types.h"
#include "../Core/Math/Math.h"

class Mesh
{
	friend class Rendering;

private:
	bgfx::VertexDecl m_vertexdecl = {};
	bgfx::VertexBufferHandle m_vertexBuffer = {};
	bgfx::IndexBufferHandle m_indexBuffer = {};

	Vector3* m_vertices = nullptr;
	Vector2* m_uvs = nullptr;
	Vector3* m_normals = nullptr;
	//vector4f* m_colors = nullptr;
	uint* m_indices = nullptr;

	uint m_vertices_count = 0u;
	uint m_uvs_count = 0u;
	uint m_normals_count = 0u;
	uint m_colors_count = 0u;
	uint m_indices_count = 0u;

private:
	void init();

public:
	/// <summary>
	/// Set vertices for this mesh.
	/// To apply changes, you must call ->applyChanges();
	/// after setting all data you need.
	/// </summary>
	/// <param name="vertices">Vertices pointer.</param>
	/// <param name="count">The count of vertices.</param>
	void setVertices(Vector3* vertices, uint count);

	/// <summary>
	/// Set uvs for this mesh.
	/// To apply changes, you must call ->applyChanges();
	/// after setting all data you need.
	/// </summary>
	/// <param name="uvs">Uvs pointer.</param>
	void setUVs(Vector2* uvs);

	/// <summary>
	/// Set normals for this mesh.
	/// To apply changes, you must call ->applyChanges();
	/// after setting all data you need.
	/// </summary>
	/// <param name="normals">Normals pointer.</param>
	void setNormals(Vector3* normals);

	/// <summary>
	/// Set colors for this mesh.
	/// To apply changes, you must call ->applyChanges();
	/// after setting all data you need.
	/// </summary>
	/// <param name="colors">Colors pointer.</param>
	//void setColors(Vector3* colors);

	/// <summary>
	/// Set indices for this mesh.
	/// To apply changes, you must call ->applyChanges();
	/// after setting all data you need.
	/// </summary>
	/// <param name="indices">Indices pointer.</param>
	/// <param name="count">The count of indices.</param>
	void setIndices(uint* indices, uint count);

	/// <summary>
	/// Apply changes.
	/// </summary>
	void applyChanges();

	/// <summary>
	/// Dispose
	/// </summary>
	void dispose();

public:
	/// <summary>
	/// Create new mesh.
	/// </summary>
	/// <returns></returns>
	static Ptr<Mesh> createMesh();

};

#endif // MESH_H