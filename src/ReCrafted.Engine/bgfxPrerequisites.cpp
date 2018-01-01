// ReCrafted (c) 2016-2018 Always Too Late

#include "bgfxPrerequisites.h"

#ifdef DEBUG
#pragma comment (lib, "bgfxDebug.lib")
#pragma comment (lib, "bxDebug.lib")

#pragma comment (lib, "bimgDebug.lib")
#pragma comment (lib, "bimg_decodeDebug.lib")
#else
#pragma comment (lib, "bgfxRelease.lib")
#pragma comment (lib, "bxRelease.lib")
#pragma comment (lib, "bimgRelease.lib")
#pragma comment (lib, "bimg_decodeRelease.lib")
#endif