// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef ATLAS_H
#define ATLAS_H

// includes
#include "Texture2D.h"
#include "Core/Math/Rect.h"

struct Text;

/// <summary>
/// Atlas class.
/// </summary>
class Atlas
{
public:
	struct Element
	{
		char name[32] = {};
		Rect rect = {};
	};

private:
	Ptr<Texture2D> m_texture = nullptr;
	std::vector<Element> m_elements = {};

private:
	Atlas() {}

public:
	/// <summary>
	/// Gets element by name.
	/// Throws error when not found.
	/// </summary>
	Atlas::Element getElement(const char* name);

	/// <summary>
	/// Gets atlas texture.
	/// </summary>
	Ptr<Texture2D> getTexture() const;
	
	/// <summary>
	/// Disposes the atlas.
	/// </summary>
	void dispose();

public:
	/// <summary>
	/// Loads atlas from JSON file.
	/// </summary>
	/// <param name="path">The JSON file name.</param>
	static Ptr<Atlas> load(Text fileName);
};

#endif // ATLAS_H
