// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef DISPLAY_H
#define DISPLAY_H

#include "ReCraftedAPI.h"
#include "../Utils/Types.h"
#include "../Utils/Defines.h"

class Display
{
	API_DEF

	public:
		STATIC_PROPERTY(uint, Width);
		STATIC_PROPERTY(uint, Height);

	public:
		FORCEINLINE static float getAspectRatio()
		{
			return static_cast<float>(get_Width()) / static_cast<float>(get_Height());
		}
};

#endif // DISPLAY_H