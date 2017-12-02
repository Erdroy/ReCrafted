// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef MESH_H
#define MESH_H

// includes
#include "bgfxPrerequisites.h"
#include "Core/Types.h"
#include "Core/Math/Math.h"

struct Vector4;

/// <summary>
/// Mesh class.
/// </summary>
class Mesh
{
	friend class Rendering;

private:
	bgfx::VertexDecl m_vertexdecl = {};
	bgfx::VertexBufferHandle m_vertexBuffer = {};
	bgfx::IndexBufferHandle m_indexBuffer = {};

	const bgfx::Memory* m_vertexBufferData = nullptr;
	const bgfx::Memory* m_indexBufferData = nullptr;

	Vector3* m_vertices = nullptr;
	Vector2* m_uvs = nullptr;
	Vector3* m_normals = nullptr;
	Vector4* m_colors = nullptr;
	uint* m_indices = nullptr;

	bool m_uploaded = false;
	bool m_hasChanges = false;

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
	void setColors(Vector4* colors);

	/// <summary>
	/// Set indices for this mesh.
	/// To apply changes, you must call ->applyChanges();
	/// after setting all data you need.
	/// </summary>
	/// <param name="indices">Indices pointer.</param>
	/// <param name="count">The count of indices.</param>
	void setIndices(uint* indices, uint count);

	/// <summary>
	/// Indicates the upload state of this mesh.
	/// </summary>
	/// <returns>True when all changes are uploaded to the GPU.</returns>
	bool isUploaded() const;

	/// <summary>
	/// Can the mesh be uploaded?
	/// </summary>
	/// <returns>True when it can be.</returns>
	bool canUpload();

	/// <summary>
	/// Apply changes.
	/// </summary>
	void applyChanges();

	void simplify();

	/// <summary>
	/// Upload buffers to the GPU.
	/// </summary>
	void upload();

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