// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#ifndef UI_H
#define UI_H

// includes
#include "../../Core/Math/Color.h"
#include "../../Core/Math/Rectf.h"

class UI
{
private:
	struct vertex
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		float u = 0.0f;
		float v = 0.0f;

		Color color = { 255, 255, 255, 255 };
	};

	struct drawcmd
	{
		vertex VertexArray[4];
		uint IndexArray[6];
		uint Texture = 0;
		Rectf ClipRect = {};
		float ZOrder = 0.0f;
		bool HasClipRect = false;
		bool HasTexture = false;
		bool HasRenderTexture = false;
	};

private:
	static UI* m_instance;

private:
	std::vector<drawcmd> m_drawCmds = {};

	bgfx::VertexDecl m_vertexdecl = {};
	bgfx::VertexBufferHandle m_vertexBuffer = {};
	bgfx::IndexBufferHandle m_indexBuffer = {};

	const bgfx::Memory* m_vertexBufferData = nullptr;
	const bgfx::Memory* m_indexBufferData = nullptr;

public:
	UI() { m_instance = this; }

public:
	void init();

	void begin_draw();
	void end_draw();

	void test_draw();
};

#endif // UI_H
