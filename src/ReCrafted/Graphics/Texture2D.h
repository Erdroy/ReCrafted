// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef TEXTURE2D_H
#define TEXTURE2D_H

// includes
#include "../Utils/Types.h"

class Texture2D
{
private:
	void load(const char* filename);

public:


public:
	/// <summary>
	/// Loads texture.
	/// </summary>
	static Ptr<Texture2D> loadTexture(const char* filename);
};

#endif // TEXTURE2D_H
