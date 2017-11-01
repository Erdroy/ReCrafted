// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef DISPLAY_H
#define DISPLAY_H

// includes
#include "ReCrafted.h"

class Display
{
	API_DEF

	public:
		STATIC_PROPERTY(uint, Width);
		STATIC_PROPERTY(uint, Height);

	public:
		FORCEINLINE static float getAspectRatio()
		{
			return static_cast<float>(m_Width) / static_cast<float>(m_Height);
		}
};

#endif // DISPLAY_H