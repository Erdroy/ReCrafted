// ReCrafted © 2016-2017 Always Too Late

#include "UI.h"
#include "Platform/Platform.h"
#include "Graphics/Rendering.h"

#include <algorithm>
#include <tuple>

UI* UI::m_instance;

bgfx::UniformHandle m_textureUnif = {};

bool UI::drawcmd_comparison(drawcmd& cmd1, drawcmd& cmd2)
{
	return std::tie(cmd1.texture, cmd1.zOrder) < std::tie(cmd2.texture, cmd2.zOrder);
}

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

	// draw
	bgfx::submit(0, m_shader->m_program);

	// reset buffer IO positions
	m_vertexBufferDataPos = 0u;
	m_indexBufferDataPos = 0u;
}

void UI::push_drawcmd(drawcmd* cmd, int index) // protip: we can use forceinline on this method because we are using it only in this source file.
{
	// push draw cmd data
	
	cmd->indices[0] += index;
	cmd->indices[1] += index;
	cmd->indices[2] += index;
	cmd->indices[3] += index;
	cmd->indices[4] += index;
	cmd->indices[5] += index;

	// copy vertex data
	auto vPtr = const_cast<byte*>(m_vertexBufferData);
	vPtr += m_vertexBufferDataPos;

	memcpy(vPtr, cmd->vertices, sizeof(vertex) * 4);

	// copy index data
	auto iPtr = const_cast<byte*>(m_indexBufferData);
	iPtr += m_indexBufferDataPos;

	memcpy(iPtr, cmd->indices, sizeof(uint) * 6);

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
	m_drawCmds = Array<drawcmd>(8 << 10);

	// crate uniform
	m_textureUnif = bgfx::createUniform("m_texture0", bgfx::UniformType::Int1);
}

void UI::dispose()
{
	// delete the buffers data
	SafeDelete(m_vertexBufferData);
	SafeDelete(m_indexBufferData);

	// dispose shader
	SafeDispose(m_shader);
}

void UI::beginDraw()
{
	// clear before drawing
	clear();
	setDepth(0.0f);
}

void UI::endDraw()
{
	// sort using zOrder (slave key) and texture (master key)
	sort(m_drawCmds.begin(), m_drawCmds.end(), drawcmd_comparison);

	auto drawCmdCount = m_drawCmds.size();
	auto vertexCount = 0;

	bgfx::TextureHandle handle;
	handle.idx = 0xFFFF;

	for (auto i = 0u; i < m_drawCmds.size(); i++)
	{
		auto drawcmd = &m_drawCmds[i];
		auto textureChanged = drawcmd->texture > 0 && handle.idx != drawcmd->texture;

		if(vertexCount + 4u > m_maxVertexCount || (textureChanged && vertexCount > 0u))
		{
			// draw now, reset, and draw more!
			drawnow();
			vertexCount = 0;
		}

		// set new texture
		if(textureChanged)
		{
			handle.idx = drawcmd->texture;
			bgfx::setTexture(0, m_textureUnif, handle);
		}

		// push draw command
		push_drawcmd(drawcmd, vertexCount);

		// increase the vertex count used for batching
		vertexCount += 4;

		// draw if this is the last command
		if(i + 1 >= drawCmdCount)
		{
			drawnow();
			break;
		}
	}

	// clear after drawing
	clear();
}

void UI::setColor(Color color)
{
	m_instance->m_color = color;
	m_instance->m_color_r = color.r / 255.0f;
	m_instance->m_color_g = color.g / 255.0f;
	m_instance->m_color_b = color.b / 255.0f;
	m_instance->m_color_a = color.a / 255.0f;
}

Color UI::getColor()
{
	return m_instance->m_color;
}

void UI::setDepth(float depth)
{
	m_instance->m_depth = depth;
}
