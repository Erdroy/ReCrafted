// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "UI.h"
#include "../../Core/GameMain.h"

void UI::drawBox(Rectf rect)
{
	auto color = Color(0xFF0000FF);

	// first vertex
	vertex v0 = {};

	// second vertex
	vertex v1 = {};

	// third vertex
	vertex v2 = {};

	// fourth vertex
	vertex v3 = {};

	// Set colors.
	v0.color = color;
	v1.color = color;
	v2.color = color;
	v3.color = color;

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
	v0.y = y;
	v0.z = 0.0f;

	// calculate x and y of the second vertex.
	v1.x = x + width;
	v1.y = y;
	v1.z = 0.0f;

	// calculate x and y of the third vertex.
	v2.x = x + width;
	v2.y = y + height;
	v2.z = 0.0f;

	// calculate x and y of the fourth vertex.
	v3.x = x;
	v3.y = y + height;
	v3.z = 0.0f;

	// build draw command
	auto cmd = drawcmd();

	cmd.vertices[0] = v0;
	cmd.vertices[1] = v1;
	cmd.vertices[2] = v2;
	cmd.vertices[3] = v3;

	// first triangle
	cmd.indices[0] = 2;
	cmd.indices[1] = 1;
	cmd.indices[2] = 0;

	// second triangle
	cmd.indices[3] = 0;
	cmd.indices[4] = 3;
	cmd.indices[5] = 2;

	m_drawCmds.push_back(cmd);
}