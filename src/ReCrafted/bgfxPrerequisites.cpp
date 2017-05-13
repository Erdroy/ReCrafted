// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "bgfxPrerequisites.h"

#ifndef NDEBUG
#pragma comment (lib, "bgfxDebug.lib")
#pragma comment (lib, "bxDebug.lib")

#pragma comment (lib, "bimgDebug.lib")
#pragma comment (lib, "bimg_decodeDebug.lib")
#pragma comment (lib, "bimg_encodeDebug.lib")
#else
#pragma comment (lib, "bgfxRelease.lib")
#pragma comment (lib, "bxRelease.lib")
#pragma comment (lib, "bimgRelease.lib")
#pragma comment (lib, "bimg_decodeRelease.lib")
#pragma comment (lib, "bimg_encodeRelease.lib")
#endif