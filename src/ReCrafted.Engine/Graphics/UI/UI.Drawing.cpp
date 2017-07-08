// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "UI.h"
#include "Core/GameMain.h"
#include "Common/Display.h"
#include "Common/Font.h"

#define BOX_VERTICES_DEFINE() vertex v0 = {}; vertex v1 = {}; vertex v2 = {}; vertex v3 = {};

#define BOX_VERTICES_SETUP() setupVertexData(rect, v0, v1, v2, v3);

#define BOX_VERTICES_FINALIZE(texture) finalizeVertexData(v0, v1, v2, v3, texture);

#define BOX_VERTICES_SETUP_UVS() v0.u = uvs.x;\
v0.v = uvs.y + uvs.height;\
v1.u = uvs.x + uvs.width;\
v1.v = uvs.y + uvs.height;\
v2.u = uvs.x + uvs.width;\
v2.v = uvs.y;\
v3.u = uvs.x;\
v3.v = uvs.y;

void UI::setupVertexData(Rectf& rect, vertex& v0, vertex& v1, vertex& v2, vertex& v3) const
{
	auto screen_width = Display::get_Width();
	auto screen_height = Display::get_Height();

	// Width
	auto width = rect.width / (screen_width * 0.5f);

	// Height
	auto height = rect.height / (screen_height * 0.5f);

	// X (Top-left)
	auto x = rect.x / (screen_width * 0.5f) - 1.0f;

	// Y (Top-left)
	auto y = 1.0f - rect.y / (screen_height * 0.5f) - height;

	// calculate x and y of the first vertex.
	v0.x = x;
	v0.y = y + height;
	v0.z = 0.0f;

	// calculate x and y of the second vertex.
	v1.x = x + width;
	v1.y = y + height;
	v1.z = 0.0f;

	// calculate x and y of the third vertex.
	v2.x = x + width;
	v2.y = y;
	v2.z = 0.0f;

	// calculate x and y of the fourth vertex.
	v3.x = x;
	v3.y = y;
	v3.z = 0.0f;

	// set colors
	v0.r = m_color_r;
	v0.g = m_color_g;
	v0.b = m_color_b;
	v0.a = m_color_a;

	v1.r = m_color_r;
	v1.g = m_color_g;
	v1.b = m_color_b;
	v1.a = m_color_a;

	v2.r = m_color_r;
	v2.g = m_color_g;
	v2.b = m_color_b;
	v2.a = m_color_a;

	v3.r = m_color_r;
	v3.g = m_color_g;
	v3.b = m_color_b;
	v3.a = m_color_a;
}

void UI::finalizeVertexData(vertex& v0, vertex& v1, vertex& v2, vertex& v3, uint texture)
{
	// build draw command
	auto cmd = drawcmd();

	if(texture > 0)
	{
		cmd.texture = texture;
		cmd.hasTexture = true;
	}
	else
		cmd.hasTexture = false;

	// set depth
	cmd.zOrder = m_depth;

	// copy vertices
	cmd.vertices[0] = v0;
	cmd.vertices[1] = v1;
	cmd.vertices[2] = v2;
	cmd.vertices[3] = v3;

	// first triangle
	cmd.indices[0] = 0;
	cmd.indices[1] = 1;
	cmd.indices[2] = 2;

	// second triangle
	cmd.indices[3] = 0;
	cmd.indices[4] = 3;
	cmd.indices[5] = 2;

	m_drawCmds.push_back(cmd);
}

void UI::internal_drawBox(Rectf rect)
{
	BOX_VERTICES_DEFINE();
	BOX_VERTICES_SETUP();

	BOX_VERTICES_FINALIZE(0);
}

void UI::internal_drawBoxTextured(Rectf rect, uint texture, Rectf& uvs)
{
	BOX_VERTICES_DEFINE();
	BOX_VERTICES_SETUP();

	BOX_VERTICES_SETUP_UVS();

	BOX_VERTICES_FINALIZE(texture);
}

void UI::setColor(Color color)
{
	m_instance->m_color = color;
	m_instance->m_color_r = color.r / 255.0f;
	m_instance->m_color_g = color.g / 255.0f;
	m_instance->m_color_b = color.b / 255.0f;
	m_instance->m_color_a = color.a / 255.0f;
}

Color UI::getColor()
{
	return m_instance->m_color;
}

void UI::drawBox(Rectf rect)
{
	rect.width -= rect.x;
	rect.height -= rect.y;

	m_instance->internal_drawBox(rect);
}

void UI::drawText(Ptr<Font> font, Text text, Vector2 position)
{
	auto currentPosition = position;
	auto lineheight = float(font->m_size) * font->m_lineHeigh;

	for (auto i = 0; i < text.length(); i++)
	{
		auto character = text[i];
		auto glyph = font->m_glyphs[character];

		if(glyph.font != font.get())
		{
			// invalid character!
			glyph = font->m_glyphs['?'];
		}

		auto glyphRect = glyph.rectangle;
		auto texture = font->m_textures[glyph.texture];

		if (character == ' ') // Space
		{
			auto glyphData = font->m_glyphs[Char('i')];
			currentPosition += Vector2(float(glyphData.advanceX), 0.0f);
		}
		else if (character == 9) // Tab
		{
			auto glyphData = font->m_glyphs[Char('i')];
			currentPosition += Vector2(float(glyphData.advanceX) * 3, 0.0f);
		}
		else if (character == '\n' || character == 10 || character == 13) // New line character.
		{
			currentPosition.x = position.x;
			currentPosition.y += lineheight;
		}
		else // A 'normal' character.
		{
			auto currentX = currentPosition.x;
			auto currentY = currentPosition.y;

			currentX += glyph.horizontalBearingX;
			currentY += lineheight;
			currentY -= glyph.horizontalBearingY;

			m_instance->internal_drawBoxTextured(
				Rectf(currentX, currentY, float(glyphRect.width), float(glyphRect.height)),
				texture->getHandle(),
				Rectf(glyphRect.x / 512.0f, (glyphRect.y + glyphRect.height) / 512.0f, glyphRect.width / 512.0f, -glyphRect.height / 512.0f));

			currentPosition += Vector2(float(glyph.advanceX), 0.0f);
		}
	}
}

/*
void UI::drawTest(Ptr<Font> font)
{
	setColor(Color(255, 255, 255));
	auto glyph = font->getCharacter(Char('X'));
	auto tex = font->m_textures[glyph.texture];

	m_instance->internal_drawBoxTextured(Rectf(200.0f, 200.0f, 512.0f, 512.0f), tex->getHandle(), Rectf(0.0f, 0.0f, 1.0f, 1.0f));
}*/
