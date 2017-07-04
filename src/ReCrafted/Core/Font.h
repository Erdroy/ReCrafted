// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef FONT_H
#define FONT_H

// includes

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

	std::vector<uint> m_textures = {};

	Glyph* m_glyphs = nullptr;
	uint m_glyphCount = 0;
	Glyph m_nullGlyph = {};

public:
	explicit Font(uint glyphCount)
	{
		m_glyphCount = glyphCount;
		m_glyphs = new Glyph[glyphCount];
	}

	~Font()
	{
		delete [] m_glyphs;
	}

public:
};

#endif // FONT_H
