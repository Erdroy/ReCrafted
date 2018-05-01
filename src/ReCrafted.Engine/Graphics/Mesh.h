// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef MESH_H
#define MESH_H

// includes
#include "Core/Types.h"
#include "Core/Math/Math.h"
#include "IResource.h"

struct Vector4;

/**
 * \brief Mesh class.
 */
class Mesh : IResource
{
    friend class Graphics;

private:
	/*bgfx::VertexDecl m_vertexdecl = {};
	bgfx::VertexBufferHandle m_vertexBuffer = {};
	bgfx::IndexBufferHandle m_indexBuffer = {};

    bgfxMemoryEx m_vertexBufferData = {};
    bgfxMemoryEx m_indexBufferData = {};*/

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

public:
    IRESOURCE_IMPL(Mesh)

private:
	void init();

public:
    /**
	 * \brief Set vertices for this mesh.
	 * \param vertices Vertices pointer.
	 * \param count The count of vertices.
	 * 
	 * \note To apply changes, you must call 'applyChanges'
	 * after setting all data you need.
	 */
	void setVertices(Vector3* vertices, uint count);

    /**
    * \brief Set uvs for this mesh.
    * \param uvs pointer.
    *
    * \note To apply changes, you must call 'applyChanges'
    * after setting all data you need.
    */
	void setUVs(Vector2* uvs);

    /**
    * \brief Set normals for this mesh.
    * \param normals pointer.
    *
    * \note To apply changes, you must call 'applyChanges'
    * after setting all data you need.
    */
	void setNormals(Vector3* normals);

    /**
    * \brief Set colors for this mesh.
    * \param colors pointer.
    *
    * \note To apply changes, you must call 'applyChanges'
    * after setting all data you need.
    */
	void setColors(Vector4* colors);

    /**
    * \brief Set colors for this mesh.
    * \param indices pointer.
	 * \param count The count of indices.
    *
    * \note To apply changes, you must call 'applyChanges'
    * after setting all data you need.
    */
	void setIndices(uint* indices, uint count);

    /**
	 * \brief Indicates the upload state of this mesh.
	 * \return True when all changes are uploaded to the GPU.
	 */
	bool isUploaded() const;

    /**
	 * \brief Can the mesh be uploaded?
	 * \return True when it can be.
	 */
	bool canUpload();

    /**
	 * \brief Apply changes.
	 */
	void applyChanges();

    /**
	 * \brief Simplifies this mesh in-place.
	 */
	void simplify();

    /**
	 * \brief Upload buffers to the GPU.
	 */
	void upload();

    /**
	 * \brief Disposes this mesh.
	 */
	void dispose() override;

public:
    /**
	 * \brief Creates new mesh.
	 * \return The newly created mesh pointer.
	 */
	static Ptr<Mesh> createMesh();
};

#endif // MESH_H