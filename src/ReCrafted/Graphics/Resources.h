// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef RESOURCES_H
#define RESOURCES_H

// includes
#include "../Graphics/Texture2D.h"
#include "../Graphics/Shader.h"

class Resources
{
private:
	Resources() {}
	~Resources() {}

public:
	static Ptr<Texture2D> getTexture(const char* name);
	static Ptr<Shader> getShader(const char* name);

	static void loadTexture(const char* fileName, const char* name);
	static void loadShader(const char* fileName, const char* name);

	static void release();
};

#endif // RESOURCES_H
