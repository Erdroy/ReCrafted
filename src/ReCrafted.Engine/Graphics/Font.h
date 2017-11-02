// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef FONT_H
#define FONT_H

// includes
#include "Core/Types.h"
#include "Core/Math/Rect.h"
#include "Core/Math/Rectf.h"
#include "Common/Text.h"

#include <vector>
#include <Scripting/Object.h>

class Texture2D;

class Font : public Object
{
	friend class UI;
	API_DEF;

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

	Array<Ptr<Texture2D>> m_textures = {};

	Glyph* m_glyphs = nullptr;
	Glyph m_nullGlyph = {};
	uint m_glyphCount = 0;

	float m_lineHeigh = 1.0f; // actually multiplier

public:
	/// <summary>
	/// Default constructor
	/// </summary>
	Font() {}

	/// <summary>
	/// Default destructor
	/// </summary>
	~Font()
	{
		if(m_glyphs)
			delete [] m_glyphs;
	}

public:
	/// <summary>
	/// Font initializer.
	/// </summary>
	/// <param name="glyphCount">The font glyph count.</param>
	void init(uint glyphCount)
	{
		m_glyphCount = glyphCount;
		m_glyphs = new Glyph[glyphCount];
		memset(m_glyphs, 0, glyphCount * sizeof Glyph);
	}

	/// <summary>
	/// Gets character glyph info struct.
	/// </summary>
	FORCEINLINE Glyph getCharacter(Char character) const
	{
		return m_glyphs[character];
	}

	/// <summary>
	/// Measures text size.
	/// </summary>
	/// <param name="text">The text to be measured.</param>
	/// <returns>The size.</returns>
	Vector2 measureText(Text text);

	/// <summary>
	/// Loads new font
	/// </summary>
	/// <param name="fontFile"></param>
	/// <param name="size"></param>
	/// <returns></returns>
	void loadFont(Text fontFile, int size, bool managed = false);
};

#endif // FONT_H
