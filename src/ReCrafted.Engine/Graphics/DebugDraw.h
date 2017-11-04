// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include "Shader.h"
#include "Core/Containers/Array.h"

struct Vector3;
struct Color;

class DebugDraw
{
	friend class GameMain;

private:
	static bgfx::VertexDecl m_vertexDecl;
	static Ptr<Shader> m_shader;

	static Array<Vector3> m_vertices;
	static Array<Vector4> m_colors;

	static uint m_vertexCount;


public:
	static void drawLine(Vector3 pointA, Vector3 pointB, Color color);

private:
	static void init();
	static void shutdown();
	static void drawAll();
	static void clean();
};

#endif // DEBUGDRAW_H
