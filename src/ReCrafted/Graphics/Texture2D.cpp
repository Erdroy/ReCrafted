// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Texture2D.h"

#define FREEIMAGE_LIB
#include "FreeImage.h"

#pragma comment(lib, "FreeImageLib.lib")

bool g_freeImage_initialized = false;

void Texture2D::load(const char* filename)
{
	auto fif = FreeImage_GetFileType(filename, 0);

	if (fif != FIF_PNG || fif != FIF_JPEG || fif != FIF_DDS || fif != FIF_BMP)
		throw;

	auto bitmap = FreeImage_Load(fif, filename);

	auto bits = FreeImage_GetBits(bitmap);
	auto width = FreeImage_GetWidth(bitmap);
	auto height = FreeImage_GetHeight(bitmap);



	FreeImage_Unload(bitmap);
}

Ptr<Texture2D> Texture2D::loadTexture(const char* filename)
{

	if(!g_freeImage_initialized)
	{
		FreeImage_Initialise();
		g_freeImage_initialized = true;
	}

	Ptr<Texture2D> texture(new Texture2D);
	texture->load(filename);
	return texture;
}
