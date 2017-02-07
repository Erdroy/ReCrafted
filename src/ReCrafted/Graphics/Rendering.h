// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef RENDERING_H
#define RENDERING_H

// includes
#include "../Utils/Defines.h"

/// <summary>
/// Rendering class.
/// </summary>
class Rendering
{
private:
	static Rendering* m_instance;

private:

public:
	Rendering() { m_instance = this; }

	/// <summary>
	/// Initialize rendering
	/// </summary>
	void init();

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