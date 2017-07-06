// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "UI.h"
#include "../../Core/Logger.h"
#include "../../Core/Math/Math.h"
#include "../../Core/Math/FastRectanglePack.h"
#include "../../Core/Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <ftglyph.h>
#pragma comment(lib, "freetype26.lib")


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

		Font::Glyph fontGlyph = {};
		if (bitmap.width != 0 && bitmap.rows != 0)
		{
			// Store glyph data.
			fontGlyph.character = glyphData.Character;
			fontGlyph.texture = charmap;
			fontGlyph.rectangle = rect;

			fontGlyph.horizontalBearingX = glyphData.HorizontalBearingX;
			fontGlyph.horizontalBearingY = glyphData.HorizontalBearingY;
			fontGlyph.advanceX = glyphData.AdvanceX;
			fontGlyph.advanceY = glyphData.AdvanceY;

			outputGlyphs.push_back(fontGlyph);
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

void UI::loadFont(Text fontFile, int size)
{
	FT_Library library = nullptr;

	auto hr = FT_Init_FreeType(&library);
	
	if(FAILED(hr))
	{
		Logger::write("Failed to init freetype library", LogLevel::Error);
		return;
	}

	auto path = fontFile.c_str().c_str();

	FT_Face face;
	hr = FT_New_Face(library, path, 0, &face);

	if (FAILED(hr))
	{
		Logger::write("Failed to load freetype font '", path, "'", LogLevel::Error);
		throw;
	}

	hr = FT_Select_Charmap(face, FT_ENCODING_UNICODE);

	if (FAILED(hr))
	{
		Logger::write("Failed to select freetype font unicode charmap '", path, "'", LogLevel::Error);
		throw;
	}

	hr = FT_Set_Pixel_Sizes(face, 0, size);

	if (FAILED(hr))
	{
		Logger::write("Failed to set freetype font size '", path, "'", LogLevel::Error);
		throw;
	}

	auto glyphsLeft = GetGlyphs(face);

	const auto charmapWidth = 512;
	const auto charmapHeight = 512;

	std::vector<PreFontGlyph> glyphsFailed = {};
	std::vector<Font::Glyph> glyphs = {};

	auto charmapId = 0;

	while (glyphsLeft.size() > 0u)
	{
		auto bits = GenerateCharmap(face, charmapId, charmapWidth, charmapHeight, glyphsLeft, glyphsFailed, glyphs);
		
		glyphsLeft.clear();
		glyphsLeft = glyphsFailed;
		glyphsFailed.clear();

		// Create alpha
		auto newbits = CreateBitmap(charmapWidth, charmapHeight, 32);
		auto newbitsPtr = newbits;

		byte *src = bits;
		byte *dst = newbits;

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

		ReleaseBitmap(bits);
		
		/*var bitmap = FreeImage_ConvertFromRawBits(newbitsPtr, charmapWidth, charmapHeight, charmapWidth * 4, 32, 0xFF0000, 0x00FF00, 0x0000FF, 0);

		// Create texture.
		r3d_obj r3dTexture;
		R3D::Do(R3D_MakeTexture, R3D_TD_Texture2D, R3D_TF_B8G8R8A8_UNORM, 1, charmapWidth, charmapHeight, static_cast<void*>(FreeImage_GetBits(bitmap)), &r3dTexture);
		font->m_textures.Add(r3dTexture);
		*/
		ReleaseBitmap(newbitsPtr);

		charmapId++;
	}

	Ptr<Font> font(new Font(glyphs.size()));
	font->m_size = size;

	// TODO: copy glyphs and textures
}
