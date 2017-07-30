// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Font.h"

// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Graphics/UI/UI.h"
#include "Graphics/Texture2D.h"
#include "Core/Logger.h"
#include "Core/Math/Math.h"
#include "Core/Math/FastRectanglePack.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>
#if _DEBUG
#pragma comment(lib, "freetype28MTd.lib")
#else
#pragma comment(lib, "freetype28MT.lib")
#endif

struct PreFontGlyph
{
	Char Character = 0;
	FT_Glyph Glyph;
	uint AdvanceY = 0;
	uint AdvanceX = 0;
	uint HorizontalBearingX = 0;
	uint HorizontalBearingY = 0;
};

std::vector<PreFontGlyph> GetGlyphs(FT_Face face)
{
	auto start = 0U;
	auto end = face->num_glyphs;

	std::vector<PreFontGlyph> glyphs = {};
	for (long index = start; index < end; index++)
	{
		auto ch = index;
		FT_Load_Char(face, ch, FT_RENDER_MODE_NORMAL);

		FT_Glyph glyph;
		FT_Get_Glyph(face->glyph, &glyph);
		//FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, 0);

		PreFontGlyph pfg = {};
		pfg.Character = static_cast<Char>(ch);
		pfg.Glyph = glyph;
		pfg.HorizontalBearingX = static_cast<uint>(Math::clamp<int>(face->glyph->metrics.horiBearingX, 0, 256) >> 6);
		pfg.HorizontalBearingY = static_cast<uint>(face->glyph->metrics.horiBearingY >> 6);
		pfg.AdvanceX = static_cast<uint>(glyph->advance.x) >> 16;
		pfg.AdvanceY = static_cast<uint>(face->glyph->linearVertAdvance) >> 16;

		glyphs.push_back(pfg);
	}

	return glyphs;
}

byte* CreateBitmap(uint width, uint height, uint bpp = 8, bool clear = true)
{
	auto length = width * height * (bpp / 8);
	auto bitmap = new byte[length];

	if (clear)
	{
		for (auto i = 0u; i < length; i++)
		{
			bitmap[i] = 0x00;
		}
	}

	return bitmap;
}

void ReleaseBitmap(byte* charmapTexure)
{
	delete[] charmapTexure;
}

byte* GenerateCharmap(FT_Face face, int charmap, uint charmapWidth, uint charmapHeight, std::vector<PreFontGlyph> glyphs, std::vector<PreFontGlyph>& failed, std::vector<Font::Glyph>& outputGlyphs)
{
	byte* texture = CreateBitmap(charmapWidth, charmapHeight);
	byte* texturePtr = texture;

	// Clear bitmap
	for (auto i = 0u; i < charmapWidth * charmapHeight; i++)
	{
		texture[i] = 0x0;
	}

	FastRectanglePack bin = {};
	bin.Init(charmapWidth, charmapHeight, 0);

	for (auto i = 0u; i < glyphs.size(); i++)
	{
		auto glyphData = glyphs[i];
		auto glyph = glyphData.Glyph;

		FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, nullptr, 0);

		auto bitmap = ((FT_BitmapGlyph)glyph)->bitmap;

		auto rect = bin.Insert(bitmap.width, bitmap.rows);

		outputGlyphs.push_back({});
		if (bitmap.width != 0 && bitmap.rows != 0)
		{
			Font::Glyph* fontGlyph = &outputGlyphs[outputGlyphs.size() - 1];
			// Store glyph data.
			fontGlyph->character = glyphData.Character;
			fontGlyph->texture = charmap;
			fontGlyph->rectangle = rect;

			fontGlyph->horizontalBearingX = glyphData.HorizontalBearingX;
			fontGlyph->horizontalBearingY = glyphData.HorizontalBearingY;
			fontGlyph->advanceX = glyphData.AdvanceX;
			fontGlyph->advanceY = glyphData.AdvanceY;
		}

		if (rect.height > 0)
		{
			auto buffer = bitmap.buffer;
			// Copy bitmap to the texture.
			for (auto y = 0u; y < bitmap.rows; y++)
			{
				int position = rect.x + charmapWidth * (y + rect.y);

				if (position + bitmap.width < charmapWidth * charmapHeight)
				{
					memcpy(texture + position, buffer + bitmap.width * y, bitmap.width);
				}
				else
				{
					// No more space
					failed.push_back(glyphData);
				}
			}
		}
		else
		{
			if (bitmap.width == 0 || bitmap.rows == 0)
			{
				// Failed.
				// Ignore this.
			}
			else
			{
				// No more space
				failed.push_back(glyphData);
			}
		}
	}

	return texturePtr;
}

Vector2 Font::measureText(Text text)
{
	Vector2 pos = {};
	auto lineheight = m_size * m_lineHeigh;

	for (auto i = 0; i < text.length(); i++)
	{
		auto character = text[i];
		auto glyph = m_glyphs[character];

		if (glyph.font != this)
		{
			// invalid character!
			glyph = m_glyphs['?'];
		}

		if (character == ' ') // Space
		{
			auto glyphData = m_glyphs[Char('i')];
			pos += Vector2(float(glyphData.advanceX), 0.0f);
		}
		else if (character == 9) // Tab
		{
			auto glyphData = m_glyphs[Char('i')];
			pos += Vector2(float(glyphData.advanceX) * 3, 0.0f);
		}
		else if (character == '\n' || character == 10 || character == 13) // New line character.
		{
			pos.x = 0.0f;
			pos.y += lineheight;
		}
		else // A 'normal' character.
		{
			pos += Vector2(float(glyph.advanceX), 0.0f);
		}
	}
	pos += Vector2(0.0f, lineheight);
	return pos;
}

void Font::loadFont(Text fontFile, int size, bool managed)
{
	FT_Library library = nullptr;

	auto hr = FT_Init_FreeType(&library);

	if (hr)
	{
		Logger::write("Failed to init freetype library", LogLevel::Error);
		return;
	}

	auto pcst = fontFile.std_str();
	auto path = pcst.c_str();

	FT_Face face;
	hr = FT_New_Face(library, path, 0, &face);

	if (hr)
	{
		Logger::write("Failed to load freetype font '", path, "'", LogLevel::Error);
		return;
	}

	hr = FT_Select_Charmap(face, FT_ENCODING_UNICODE);

	if (hr)
	{
		Logger::write("Failed to select freetype font unicode charmap '", path, "'", LogLevel::Error);
		return;
	}

	hr = FT_Set_Pixel_Sizes(face, 0, size);

	if (hr)
	{
		Logger::write("Failed to set freetype font size '", path, "'", LogLevel::Error);
		return;
	}

	auto glyphsLeft = GetGlyphs(face);

	const auto charmapWidth = 512;
	const auto charmapHeight = 512;

	std::vector<PreFontGlyph> glyphsFailed = {};
	std::vector<Font::Glyph> glyphs = {};

	auto charmapId = 0;

	auto textures = std::vector<Ptr<Texture2D>>();

	while (glyphsLeft.size() > 0u)
	{
		auto bits = GenerateCharmap(face, charmapId, charmapWidth, charmapHeight, glyphsLeft, glyphsFailed, glyphs);

		glyphsLeft.clear();
		glyphsLeft = glyphsFailed;
		glyphsFailed.clear();

		// create alpha
		auto newbits = CreateBitmap(charmapWidth, charmapHeight, 32);
		auto newbitsPtr = newbits;

		auto src = bits;
		auto dst = newbits;

		for (uint j = 0; j < static_cast<uint>(charmapWidth * charmapHeight); j++)
		{
			auto A = *src++;

			if (A > 0x01)
			{
				*dst++ = 0xFF;	// R
				*dst++ = 0xFF;	// G
				*dst++ = 0xFF;	// B
				*dst++ = A;		// A
			}
			else
			{
				*dst++ = 0x00;	// R
				*dst++ = 0x00;	// G
				*dst++ = 0x00;	// B
				*dst++ = 0x00;	// A
			}
		}

		// close
		ReleaseBitmap(bits);

		// swap-Y
		for (auto i = 0; i < charmapWidth; i++)
		{
			for (auto j = 0; j < charmapHeight; j++)
			{
				auto tmp = newbitsPtr[j + (charmapWidth - 1 - i) * charmapHeight];
				newbitsPtr[j + (charmapWidth - 1 - i) * charmapHeight] = newbitsPtr[j + i * charmapWidth];
				newbitsPtr[j + i * charmapWidth] = tmp;
			}
		}

		// create texture
		auto texture = Texture2D::createTexture();
		texture->loadMemory(newbitsPtr, charmapWidth, charmapHeight);
		texture->apply();
		textures.push_back(texture);

		// close
		ReleaseBitmap(newbitsPtr);

		charmapId++;
	}

	init(static_cast<uint>(glyphs.size()));
	
	m_size = size;
	m_textures = textures;
	m_charmapWidth = charmapWidth;
	m_charmapHeight = charmapHeight;
	m_nullGlyph = getCharacter(Char('?'));

	for (auto && gl : glyphs)
		gl.font = this;

	memcpy(m_glyphs, glyphs.data(), glyphs.size() * sizeof Glyph);

}
