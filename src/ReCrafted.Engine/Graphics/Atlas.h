// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef ATLAS_H
#define ATLAS_H

// includes
#include "Texture2D.h"

struct Text;

class Atlas
{
	struct Element
	{
		
	};

private:
	Ptr<Texture2D> m_texture = nullptr;

private:
	Atlas() {}
public:
	void init();
	void dispose();

public:
	/// <summary>
	/// Loads atlas from JSON file.
	/// </summary>
	/// <param name="path">The JSON file name.</param>
	static Ptr<Atlas> load(Text fileName);
};

#endif // ATLAS_H
