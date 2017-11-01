// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef FASTRECTANGLEPACK_H
#define FASTRECTANGLEPACK_H

// includes
#include "Rect.h"

/// <summary>
/// Fast rectangle pack - Algorithm for packing rectangles really fast.
/// Works best if the rectangles are sorted using their Height as key.
/// </summary>
class FastRectanglePack
{
	int m_width = 0;
	int m_height = 0;

	int m_padding = 0;

	int m_currentMaxHeight = 0;
	int m_currentWidth = 0;
	int m_currentHeight = 0;

public:

	/// <summary>
	/// Initialize the rectangle map.
	/// </summary>
	/// <param name="width">The maximum width which will be used.</param>
	/// <param name="height">The maximum height which will be used.</param>
	/// <param name="padding">Distance between rectangles.</param>
	void Init(int width, int height, int padding = 0)
	{
		m_width = width;
		m_height = height;
		m_padding = padding;
	}

	/// <summary>
	/// Inserts new item.
	/// </summary>
	Rect Insert(int width, int height)
	{
		auto rect = Rect(0, 0, width, height);

		if (height > 0)
		{
			// Find the highest 'height' for this row.
			if (height + m_padding > m_currentMaxHeight)
				m_currentMaxHeight = height + m_padding;

			if (m_currentWidth + width + m_padding > m_width)
			{
				// Check if there is more space.
				if (m_currentHeight + m_currentMaxHeight * 2 > m_height)
				{
					// No more space.
					return Rect(0, 0, 0, 0);
				}

				// Create new row.
				m_currentHeight += m_currentMaxHeight;
				m_currentWidth = 0;
				m_currentMaxHeight = 0;
			}

			rect.x = m_currentWidth;
			rect.y = m_currentHeight;

			m_currentWidth += width + m_padding;

			// Rect ok.
			return rect;
		}

		// Invalid rect
		return Rect(0, 0, 0, 0);
	}
};

#endif // FASTRECTANGLEPACK_H
