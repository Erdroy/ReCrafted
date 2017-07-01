// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "UI.h"
#include "../../Platform/Platform.h"

UI* UI::m_instance;

void UI::clear()
{
	// clear draw command array
	m_drawCmds.clear();

	// reset flags?
}

void UI::drawnow()
{
	// reference buffers
	auto vbdPtr = bgfx::makeRef(m_vertexBufferData, m_vertexBufferSize);
	auto ibdPtr = bgfx::makeRef(m_indexBufferData, m_indexBufferSize);

	// TODO: update buffers

	// TODO: draw

	// TODO: reset buffer IO positions

}

void UI::init()
{
	// allocate vertex buffer data
	m_vertexBufferData = new byte[m_vertexBufferSize];

	// allocate index buffer data
	m_indexBufferData = new byte[m_indexBufferSize];

	// clear data
	Platform::zeroMemory(const_cast<byte*>(m_vertexBufferData), m_vertexBufferSize);
	Platform::zeroMemory(const_cast<byte*>(m_indexBufferData), m_indexBufferSize);

	// create vertex description
	m_vertexdecl = {};
	m_vertexdecl.begin();
	m_vertexdecl.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);
	m_vertexdecl.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float);
	m_vertexdecl.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float);
	m_vertexdecl.end();

	// create dynamic buffers
	m_vertexBuffer = bgfx::createDynamicVertexBuffer(m_maxVertexCount, m_vertexdecl, BGFX_BUFFER_NONE);
	m_indexBuffer = bgfx::createDynamicIndexBuffer(m_maxIndexCount, BGFX_BUFFER_NONE);

	// allocate draw command for first upload (it's 1/4 of max vertex count as there is 4 vertexes per command)
	m_drawCmds = std::vector<drawcmd>(8 << 10);
}

void UI::dispose()
{
	// delete the buffers data
	SafeDelete(m_vertexBufferData);
	SafeDelete(m_indexBufferData);
}

void UI::beginDraw()
{
	// clear before drawing
	clear();
}

void UI::endDraw()
{
	// TODO: sorting

	auto vertexCount = 0;
	for (auto i = 0u; i < m_drawCmds.size(); i++)
	{
		if(vertexCount + 4 > m_maxVertexCount)
		{
			// draw now, reset, and draw more!
			drawnow();
			vertexCount = 0;
		}

		auto drawcmd = &m_drawCmds[i];

		// push draw command
		push_drawcmd(drawcmd);

		// increase the vertex count used for batching
		vertexCount += 4;
	}

	// clear after drawing
	clear();
}

void UI::testDraw()
{
	drawcmd cmd = {};

	// add draw command
	m_drawCmds.push_back(cmd);
}
