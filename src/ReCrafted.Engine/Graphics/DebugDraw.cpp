// ReCrafted © 2016-2017 Always Too Late

#include "DebugDraw.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Color.h"
#include "Camera.h"
#include "Rendering.h"

bgfx::VertexDecl DebugDraw::m_vertexDecl;
Ptr<Shader> DebugDraw::m_shader;

Array<Vector3> DebugDraw::m_vertices;
Array<Vector4> DebugDraw::m_colors;
Array<uint16_t> DebugDraw::m_indices;

void DebugDraw::drawLine(Vector3 pointA, Vector3 pointB, Color color)
{
	if (m_indices.count() + 2 >= 65535)
		return; // we cannot draw more!

	auto index = m_vertices.count();

	m_vertices.add(pointA);
	m_vertices.add(pointB);

	m_colors.add(Vector4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f));
	m_colors.add(Vector4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f));

	m_indices.add(index + 0);
	m_indices.add(index + 1);
}

void DebugDraw::init()
{
	// initialize arrays
	m_vertices = Array<Vector3>(8192);
	m_colors = Array<Vector4>(8192);

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
	if (m_vertices.count() == 0u)
	{
		// clean
		clean();
		return;
	}

	// alloc transient buffers
	bgfx::TransientVertexBuffer vertexBuffer;
	bgfx::allocTransientVertexBuffer(&vertexBuffer, m_vertices.count(), m_vertexDecl);
	bgfx::TransientIndexBuffer indexBuffer;
	bgfx::allocTransientIndexBuffer(&indexBuffer, m_indices.count());
	
	// upload data
	auto memoryPtr = vertexBuffer.data;

	for(auto i = 0u; i < m_vertices.count(); i ++)
	{
		auto offset = i * m_vertexDecl.getStride();
		auto vert = m_vertices[i];
		auto col = m_colors[i];

		memcpy(memoryPtr + offset, &vert, sizeof(float) * 3);
		memcpy(memoryPtr + offset + 3 * sizeof(float), &col, sizeof(float) * 4);
	}

	memcpy(indexBuffer.data, m_indices.data(), sizeof(uint16_t) * m_indices.count());

	// draw buffer
	bgfx::setVertexBuffer(0, &vertexBuffer);
	bgfx::setIndexBuffer(&indexBuffer);

	auto modelMatrix = Matrix::identity();
	Rendering::getInstance()->setWorldMatrice(modelMatrix);

	bgfx::submit(0, m_shader->getProgram());

	// clean
	clean();
}

void DebugDraw::clean()
{
	// clean
	m_vertices.clear();
	m_colors.clear();
	m_indices.clear();
}
