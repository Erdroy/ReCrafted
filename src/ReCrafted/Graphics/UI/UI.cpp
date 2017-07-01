// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "UI.h"
#include "../../Platform/Platform.h"

UI* UI::m_instance;

const unsigned int m_maxVertexCount = 4 * (8 << 10);
const unsigned int m_maxIndexCount = 6 * (8 << 10);

const int m_vertexBufferSize = m_maxVertexCount * sizeof ui_vertex;
const int m_indexBufferSize = m_maxIndexCount * sizeof uint;

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

	// TODO: clear buffer IO positions

}

void UI::push_drawcmd(ui_drawcmd* cmd)
{
	// push draw cmd data

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
	m_drawCmds = std::vector<ui_drawcmd>(8 << 10);
}

void UI::dispose()
{
	// delete the buffers data
	SafeDelete(m_vertexBufferData);
	SafeDelete(m_indexBufferData);
}

void UI::begin_draw()
{
	// clear before drawing
	clear();
}

void UI::end_draw()
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

void UI::test_draw()
{
	ui_drawcmd cmd = {};

	// add draw command
	m_drawCmds.push_back(cmd);
}
