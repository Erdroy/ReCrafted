// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef FONT_H
#define FONT_H

// includes
#include "Utils/Types.h"
#include "Core/Math/Rect.h"
#include "Common/Text.h"
#include <vector>

class Texture2D;

class Font
{
	friend class UI;

public:
	struct Glyph
	{
		Font* font;

		Char character = 0;
		uint texture = 0;
		Rect rectangle = {};
		uint advanceY = 0;
		uint advanceX = 0;
		uint horizontalBearingX = 0;
		uint horizontalBearingY = 0;
	};

private:
	uint m_size = 0;
	uint m_charmapWidth = 512;
	uint m_charmapHeight = 512;

	std::vector<Ptr<Texture2D>> m_textures = {};

	Glyph* m_glyphs = nullptr;
	Glyph m_nullGlyph = {};
	uint m_glyphCount = 0;

	float m_lineHeigh = 1.0f; // actually multiplier

public:
	/// <summary>
	/// Default constructor
	/// </summary>
	explicit Font(uint glyphCount)
	{
		m_glyphCount = glyphCount;
		m_glyphs = new Glyph[glyphCount];
		memset(m_glyphs, 0, glyphCount * sizeof Glyph);
	}

	/// <summary>
	/// Default destructor
	/// </summary>
	~Font()
	{
		delete [] m_glyphs;
	}

public:
	/// <summary>
	/// Gets character glyph info struct.
	/// </summary>
	FORCEINLINE Glyph getCharacter(Char character) const
	{
		return m_glyphs[character];
	}

	/// <summary>
	/// Loads new font
	/// </summary>
	/// <param name="fontFile"></param>
	/// <param name="size"></param>
	/// <returns></returns>
	static Ptr<Font> loadFont(Text fontFile, int size);
};

#endif // FONT_H
