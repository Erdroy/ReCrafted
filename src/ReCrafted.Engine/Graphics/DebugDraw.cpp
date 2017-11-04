// ReCrafted © 2016-2017 Always Too Late

#include "DebugDraw.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Color.h"
#include "Camera.h"
#include "Rendering.h"

bgfx::VertexDecl DebugDraw::m_vertexDecl;
Ptr<Shader> DebugDraw::m_shader;

Array<DebugDraw::drawlist> DebugDraw::m_drawLists;
DebugDraw::drawlist DebugDraw::m_currentLines;
DebugDraw::drawlist DebugDraw::m_currentTriangles;

Color DebugDraw::m_color;

Vector3 m_wireCubeEdges[12][2] = {
	{
		Vector3(-1.0f, -1.0f, 1.0f),
		Vector3(-1.0f, -1.0f, -1.0f),
	},
	{
		Vector3(1.0f, -1.0f, 1.0f),
		Vector3(1.0f, -1.0f, -1.0f),
	},
	{
		Vector3(1.0f, -1.0f, 1.0f),
		Vector3(-1.0f, -1.0f, 1.0f),
	},
	{
		Vector3(1.0f, -1.0f, -1.0f),
		Vector3(-1.0f, -1.0f, -1.0f),
	},

	{
		Vector3(-1.0f, 1.0f, 1.0f),
		Vector3(-1.0f, 1.0f, -1.0f),
	},
	{
		Vector3(1.0f, 1.0f, 1.0f),
		Vector3(1.0f, 1.0f, -1.0f),
	},
	{
		Vector3(1.0f, 1.0f, 1.0f),
		Vector3(-1.0f, 1.0f, 1.0f),
	},
	{
		Vector3(1.0f, 1.0f, -1.0f),
		Vector3(-1.0f, 1.0f, -1.0f),
	},

	{
		Vector3(-1.0f, -1.0f, 1.0f),
		Vector3(-1.0f, 1.0f, 1.0f),
	},
	{
		Vector3(1.0f, -1.0f, 1.0f),
		Vector3(1.0f, 1.0f, 1.0f),
	},
	{
		Vector3(1.0f, -1.0f, -1.0f),
		Vector3(1.0f, 1.0f, -1.0f),
	},
	{
		Vector3(-1.0f, -1.0f, -1.0f),
		Vector3(-1.0f, 1.0f, -1.0f),
	}
};

Vector3 m_cubeVertices[8] = {
	Vector3(-1.0f, 1.0f, -1.0f), // 0  b---
	Vector3(-1.0f, 1.0f,  1.0f), // 1  f---
	Vector3( 1.0f, 1.0f,  1.0f), // 2  ---f
	Vector3( 1.0f, 1.0f, -1.0f), // 3  ---b

	Vector3(-1.0f, -1.0f, -1.0f), // 4  f---
	Vector3(-1.0f, -1.0f,  1.0f), // 5  b---
	Vector3( 1.0f, -1.0f,  1.0f), // 6  ---f
	Vector3( 1.0f, -1.0f, -1.0f), // 7  ---b
};

uint16_t m_cubeIndices[36] = {
	0, 1, 2, 2, 3, 0, // top
	6, 5, 4, 4, 7, 6, // bottom

	7, 4, 0, 0, 3, 7, // front
	1, 5, 6, 6, 2, 1, // back

	1, 0, 5, 0, 4, 5, // left
	6, 7, 2, 7, 3, 2  // right
};

void DebugDraw::drawLine(Vector3 pointA, Vector3 pointB)
{
	if (m_currentLines.isFull())
	{
		// push this list
		m_drawLists.add(m_currentLines);

		// and make another
		m_currentLines = {};
		m_currentLines.lines = true;
		return;
	}

	auto index = m_currentLines.m_vertices.count();

	m_currentLines.m_vertices.add(pointA);
	m_currentLines.m_vertices.add(pointB);

	m_currentLines.m_colors.add(Vector4(m_color.r / 255.0f, m_color.g / 255.0f, m_color.b / 255.0f, m_color.a / 255.0f));
	m_currentLines.m_colors.add(Vector4(m_color.r / 255.0f, m_color.g / 255.0f, m_color.b / 255.0f, m_color.a / 255.0f));

	m_currentLines.m_indices.add(index + 0);
	m_currentLines.m_indices.add(index + 1);
}

void DebugDraw::drawTriangles(Vector3* vertices, uint16_t* indices, uint16_t vertexCount, uint16_t indexCount)
{
	if (m_currentTriangles.isFull())
	{
		// push this list
		m_drawLists.add(m_currentTriangles);

		// and make another
		m_currentTriangles = {};
		m_currentTriangles.lines = false;
		return;
	}

	auto index = m_currentTriangles.m_vertices.count();

	auto color = Vector4(m_color.r / 255.0f, m_color.g / 255.0f, m_color.b / 255.0f, m_color.a / 255.0f);

	for (auto i = 0; i < vertexCount; i++) 
	{
		m_currentTriangles.m_vertices.add(vertices[i]);
		m_currentTriangles.m_colors.add(color);
	}

	for (auto i = 0; i < indexCount; i++)
		m_currentTriangles.m_indices.add(index + indices[i]);

}

void DebugDraw::setColor(Color color)
{
	m_color = color;
}

void DebugDraw::drawWireCube(Vector3 center, Vector3 size)
{
	auto halfSize = size * 0.5f;

	for(auto i = 0; i < 12; i ++)
		drawLine(center + m_wireCubeEdges[i][0] * halfSize, center + m_wireCubeEdges[i][1] * halfSize);
}

void DebugDraw::drawCube(Vector3 center, Vector3 size)
{
	auto halfSize = size * 0.5f;

	Vector3 vertices[36];

	for(auto i = 0; i < 36; i ++)
	{
		vertices[i] = center + m_cubeVertices[i] * halfSize;
	}

	drawTriangles(vertices, m_cubeIndices, 8, 36);
}

void DebugDraw::drawSphere(Vector3 center, float radius)
{
	// TODO: mesh
}

void DebugDraw::init()
{
	clean();

	// load shader
	m_shader = Shader::loadShader("default_debug");

	// create vertex description
	m_vertexDecl = {};
	m_vertexDecl.begin();
	m_vertexDecl.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);
	m_vertexDecl.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float);
	m_vertexDecl.end();
}

void DebugDraw::shutdown()
{
	clean();
	
	m_shader->dispose();
}

void DebugDraw::drawAll()
{
	if(m_currentLines.m_indices.count() > 0)
		m_drawLists.add(m_currentLines);

	if (m_currentTriangles.m_indices.count() > 0)
		m_drawLists.add(m_currentTriangles);

	if(m_drawLists.count() == 0)
	{
		clean();
		return;
	}

	for(auto && list : m_drawLists)
	{
		drawDrawlist(list);
	}

	// clean
	clean();
}

void DebugDraw::drawDrawlist(drawlist& drawlist)
{
	if(drawlist.lines)
	{
		Rendering::getInstance()->setState(false, false, false, true, false);
	}
	else
	{
		Rendering::getInstance()->setState(false, false, false, false, true);
	}

	// alloc transient buffers
	bgfx::TransientVertexBuffer vertexBuffer;
	bgfx::allocTransientVertexBuffer(&vertexBuffer, drawlist.m_vertices.count(), m_vertexDecl);
	bgfx::TransientIndexBuffer indexBuffer;
	bgfx::allocTransientIndexBuffer(&indexBuffer, drawlist.m_indices.count());

	// upload data
	auto memoryPtr = vertexBuffer.data;

	for (auto i = 0u; i < drawlist.m_vertices.count(); i++)
	{
		auto offset = i * m_vertexDecl.getStride();
		auto vert = drawlist.m_vertices[i];
		auto col = drawlist.m_colors[i];

		memcpy(memoryPtr + offset, &vert, sizeof(float) * 3);
		memcpy(memoryPtr + offset + 3 * sizeof(float), &col, sizeof(float) * 4);
	}

	memcpy(indexBuffer.data, drawlist.m_indices.data(), sizeof(uint16_t) * drawlist.m_indices.count());

	// draw buffer
	bgfx::setVertexBuffer(0, &vertexBuffer);
	bgfx::setIndexBuffer(&indexBuffer);

	auto modelMatrix = Matrix::identity();
	Rendering::getInstance()->setWorldMatrice(modelMatrix);

	bgfx::submit(0, m_shader->getProgram());
}

void DebugDraw::clean()
{
	m_currentLines = {};
	m_currentLines.lines = true;

	m_currentTriangles = {};
	m_currentTriangles.lines = false;

	m_drawLists.clear();
}
