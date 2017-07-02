// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "UI.h"
#include "../../Platform/Platform.h"
#include "../Rendering.h"

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

	// update buffers
	bgfx::updateDynamicVertexBuffer(m_vertexBuffer, 0u, vbdPtr);
	bgfx::updateDynamicIndexBuffer(m_indexBuffer, 0u, ibdPtr);

	auto vertexCount = m_vertexBufferDataPos / sizeof vertex;
	auto indexCount = m_indexBufferDataPos / sizeof uint;

	// set buffers
	bgfx::setVertexBuffer(0, m_vertexBuffer, 0u, uint(vertexCount));
	bgfx::setIndexBuffer(m_indexBuffer, 0u, uint(indexCount));

	// TODO: set texture

	// draw
	bgfx::submit(0, m_shader->m_program);

	// reset buffer IO positions
	m_vertexBufferDataPos = 0u;
	m_indexBufferDataPos = 0u;
}

void UI::push_drawcmd(drawcmd* cmd) // protip: we can use forceinline on this method because we are using it only in this source file.
{
	// push draw cmd data
	
	// copy vertex data
	auto vPtr = const_cast<byte*>(m_vertexBufferData);
	memcpy(vPtr + m_vertexBufferDataPos, cmd->vertices, sizeof(vertex) * 4);

	// copy index data
	auto iPtr = const_cast<byte*>(m_indexBufferData);
	memcpy(iPtr + m_indexBufferDataPos, cmd->indices, sizeof(uint) * 6);

	// increase data pos
	m_vertexBufferDataPos += sizeof(vertex) * 4;
	m_indexBufferDataPos += sizeof(uint) * 6;
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
	m_vertexBuffer = bgfx::createDynamicVertexBuffer(m_maxVertexCount, m_vertexdecl);
	m_indexBuffer = bgfx::createDynamicIndexBuffer(m_maxIndexCount, BGFX_BUFFER_INDEX32);

	// load shader
	m_shader = Shader::loadShader("default_ui");

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
		if(vertexCount + 4u > m_maxVertexCount) // or texture changes
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

		// draw if this is the last command
		if(i + 1 == m_drawCmds.size())
		{
			drawnow();
			break;
		}
	}

	// clear after drawing
	clear();
}

void UI::testDraw()
{
	setColor(Color(255, 0, 255));
	drawBox(Rectf(0.0f, 0.0f, 100.0f, 100.0f));
}
