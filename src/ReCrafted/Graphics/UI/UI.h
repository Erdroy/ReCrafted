// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#ifndef UI_H
#define UI_H

// includes
#include "../../Core/Math/Color.h"
#include "../../Core/Math/Rectf.h"
#include "../Shader.h"

class UI
{
private:
	ALIGN(4) struct vertex
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		float u = 0.0f;
		float v = 0.0f;

		Color color = Color(0xFFFFFFFF); // hex 255, 255, 255, 255
	};

	struct drawcmd
	{
		vertex vertices[4];
		uint indices[6];
		Rectf clipRect = {};
		uint texture = 0;
		float zOrder = 0.0f;
		bool hasClipRect = false;
		bool hasTexture = false;
		bool hasRenderTexture = false;
	};

private:
	const unsigned int m_maxVertexCount = 4 * (8 << 10);
	const unsigned int m_maxIndexCount = 6 * (8 << 10);

	const int m_vertexBufferSize = m_maxVertexCount * sizeof vertex;
	const int m_indexBufferSize = m_maxIndexCount * sizeof uint;

private:
	static UI* m_instance;

private:
	std::vector<drawcmd> m_drawCmds = {};

	bgfx::VertexDecl m_vertexdecl = {};
	bgfx::DynamicVertexBufferHandle m_vertexBuffer = {};
	bgfx::DynamicIndexBufferHandle m_indexBuffer = {};

	const byte* m_vertexBufferData = nullptr;
	const byte* m_indexBufferData = nullptr;

	uint m_vertexBufferDataPos = 0u;
	uint m_indexBufferDataPos = 0u;

	Ptr<Shader> m_shader = nullptr;

private:
	void clear();

	void drawnow();

	FORCEINLINE void push_drawcmd(drawcmd* cmd);

public:
	UI() { m_instance = this; }

public:
	void init();

	void dispose();
	
	void beginDraw();

	void endDraw();

	void testDraw();

	void drawBox(Rectf rect);

};

#endif // UI_H
