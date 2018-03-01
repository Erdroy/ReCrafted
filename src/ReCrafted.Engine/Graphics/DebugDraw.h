// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include "Shader.h"
#include "Core/Containers/Array.h"
#include "Core/Math/Color.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"

struct Vector3;
struct Color;

class DebugDraw
{
    friend class GameMain;

private:
	SCRIPTING_API_IMPL()


private:
	struct drawlist
	{
		Array<Vector3> m_vertices;
		Array<Vector4> m_colors;
		Array<uint16_t> m_indices;

		drawlist()
		{
			m_vertices = Array<Vector3>(8192);
			m_colors = Array<Vector4>(8192);
			m_indices = Array<uint16_t>(8192);
		}

		bool lines = false;

		bool isFull() const
		{
			return m_indices.count() + 1 > 65530;
		}
	};

private:
	static bgfx::VertexDecl m_vertexDecl;
	static Ptr<Shader> m_shader;

	static Array<drawlist> m_drawLists;
	static drawlist m_currentLines;
	static drawlist m_currentTriangles;

	static Color m_color;

public:
	static void setColor(Color color);
	static Color* getColor();

	static void drawLine(Vector3 pointA, Vector3 pointB);
	static void drawTriangles(Vector3* vertices, uint16_t* indices, uint16_t vertexCount, uint16_t indexCount);

	static void drawWireCube(Vector3 center, Vector3 size);

	static void drawCube(Vector3 center, Vector3 size);

private:
	static void init();
	static void shutdown();
	static void drawAll();
	static void drawDrawlist(drawlist& drawlist);
	static void clean();
};

#endif // DEBUGDRAW_H
