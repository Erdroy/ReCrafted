// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef RENDERING_H
#define RENDERING_H

// includes
#include "../Utils/Defines.h"
#include "../Utils/Types.h"
#include "Mesh.h"
#include "Shader.h"
#include "../Core/Math/Math.h"
#include "RenderBuffer.h"

/// <summary>
/// Rendering class.
/// </summary>
class Rendering
{
private:
	static Rendering* m_instance;

private:
	Ptr<Mesh> m_testMesh = nullptr;
	Ptr<Shader> m_testShader = nullptr;

	Ptr<RenderBuffer> m_gbuffer = nullptr;

public:
	Rendering() { m_instance = this; }

	/// <summary>
	/// Initialize rendering
	/// </summary>
	void init();

	/// <summary>
	/// Resize
	/// </summary>
	/// <param name="width">Width</param>
	/// <param name="height">Height</param>
	void resize(uint width, uint height);

	/// <summary>
	/// Begin scene render
	/// </summary>
	void beginRender();

	/// <summary>
	/// End scene render
	/// </summary>
	void endRender();

	/// <summary>
	/// Render shadowmap
	/// </summary>
	void renderShadows();

	/// <summary>
	/// Render static objects - voxels etc.
	/// </summary>
	void renderStatic();

	/// <summary>
	/// Render all dynamic entities.
	/// </summary>
	void renderEntities();

	/// <summary>
	/// Draw mesh.
	/// </summary>
	/// <param name="mesh">Mesh to draw.</param>
	/// <param name="shader">Shader that will be used to shade the mesh.</param>
	/// <param name="modelMatrix">Matrix which includes translation, rotation and scale of this mesh for rendering.</param>
	void draw(Ptr<Mesh> mesh, Ptr<Shader> shader, Matrix* modelMatrix = nullptr);

	/// <summary>
	/// Dispose everything.
	/// </summary>
	void dispose();

public:
	/// <summary>
	/// Get instance of current rendering instance.
	/// </summary>
	/// <returns>The current rendering instance.</returns>
	FORCEINLINE static Rendering* getInstance()
	{
		return m_instance;
	}
};

#endif // RENDERING_H