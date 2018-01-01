// ReCrafted (c) 2016-2018 Always Too Late

#include "UI.h"
#include "Platform/Platform.h"
#include "Graphics/Rendering.h"

#include <algorithm>
#include <tuple>

UI* UI::m_instance;

bgfx::UniformHandle m_textureUnif = {};

bool UI::drawcmd_comparison(drawcmd& cmd1, drawcmd& cmd2)
{
	return std::tie(cmd1.zOrder, cmd1.texture) < std::tie(cmd2.zOrder, cmd2.texture);
}

void UI::clear()
{
	// clear draw command array
	m_drawCmds.clear();

	// reset flags?
}

void UI::drawnow()
{
	// allocate transient buffers
	bgfx::TransientVertexBuffer tvb;
	bgfx::TransientIndexBuffer tib;

	if(!bgfx::allocTransientBuffers(&tvb, m_vertexdecl, m_vertexCount, &tib, m_indexCount))
		return;

	// upload data
	memcpy(tvb.data, m_vertexBufferData, m_vertexCount * sizeof(vertex));
	memcpy(tib.data, m_indexBufferData, m_indexCount * sizeof(uint16_t));

	// set state
	Rendering::getInstance()->setState(false, false, true);

	// set buffers
	bgfx::setVertexBuffer(0, &tvb, 0, m_vertexCount);
	bgfx::setIndexBuffer(&tib, 0, m_indexCount);

	// draw
	bgfx::submit(0, m_shader->m_program);

	// reset buffer IO positions
	m_vertexBufferDataPos = 0u;
	m_indexBufferDataPos = 0u;
	m_vertexCount = 0u;
	m_indexCount = 0;
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

	memcpy(iPtr, cmd->indices, sizeof(uint16_t) * 6);

	// increase data pos
	m_vertexBufferDataPos += sizeof(vertex) * 4;
	m_indexBufferDataPos += sizeof(uint16_t) * 6;

	m_vertexCount += 4;
	m_indexCount += 6;
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
	m_vertexdecl.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float);
	m_vertexdecl.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float);
	m_vertexdecl.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float);
	m_vertexdecl.end();

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

	bgfx::TextureHandle textureHandle;
	textureHandle.idx = 0xFFFF;

	for (auto i = 0u; i < m_drawCmds.size(); i++)
	{
		auto drawcmd = &m_drawCmds[i];
		auto textureChanged = drawcmd->texture > 0 && textureHandle.idx != drawcmd->texture;

		if(vertexCount + 4u > m_maxVertexCount || (textureChanged && vertexCount > 0u))
		{
			// draw now, reset, and draw more!
			drawnow();
			vertexCount = 0;
		}

		// set new texture
		if(textureChanged)
		{
			textureHandle.idx = drawcmd->texture;
			bgfx::setTexture(0, m_textureUnif, textureHandle);
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

float UI::getDepth()
{
	return m_instance->m_depth;
}

void UI::setViewRect(Rectf* viewRect, bool enabled)
{
    m_instance->m_viewRect = *viewRect;
    m_instance->m_useViewRect = enabled;
}
