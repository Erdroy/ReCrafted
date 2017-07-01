// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#ifndef UI_H
#define UI_H

// includes
#include "../../Core/Math/Color.h"
#include "../../Core/Math/Rectf.h"

ALIGN(4) struct ui_vertex
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	float u = 0.0f;
	float v = 0.0f;

	Color color = Color(0xFFFFFFFF); // hex 255, 255, 255, 255
};

struct ui_drawcmd
{
	ui_vertex vertices[4];
	uint indices[6];
	Rectf clipRect = {};
	uint texture = 0;
	float zOrder = 0.0f;
	bool hasClipRect = false;
	bool hasTexture = false;
	bool hasRenderTexture = false;
};

class UI
{
private:
	static UI* m_instance;

private:
	std::vector<ui_drawcmd> m_drawCmds = {};

	bgfx::VertexDecl m_vertexdecl = {};
	bgfx::DynamicVertexBufferHandle m_vertexBuffer = {};
	bgfx::DynamicIndexBufferHandle m_indexBuffer = {};

	const byte* m_vertexBufferData = nullptr;
	const byte* m_indexBufferData = nullptr;

private:
	void clear();

	void drawnow();

	FORCEINLINE void push_drawcmd(ui_drawcmd* cmd);

public:
	UI() { m_instance = this; }

public:
	void init();

	void dispose();
	
	void begin_draw();
	void end_draw();

	void test_draw();

};

#endif // UI_H
