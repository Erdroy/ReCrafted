// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "UI.h"
#include "../../Core/GameMain.h"

void UI::drawBox(Rectf rect)
{
	// first vertex
	vertex v0 = {};

	// second vertex
	vertex v1 = {};

	// third vertex
	vertex v2 = {};

	// fourth vertex
	vertex v3 = {};

	auto screen_width = GameMain::getWindowWidth();
	auto screen_height = GameMain::getWindowHeight();

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

	// build draw command
	auto cmd = drawcmd();

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
