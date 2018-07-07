// ReCrafted (c) 2016-2018 Always Too Late

#include "UI.h"
#include "Graphics/Graphics.h"

#include <algorithm>
#include <tuple>

SINGLETON_IMPL(UI)

bool UI::drawcmd_comparison(drawcmd& cmd1, drawcmd& cmd2)
{
    return std::tie(cmd1.zOrder, cmd1.texture) < std::tie(cmd2.zOrder, cmd2.texture);
}

void UI::Clear()
{
    // clear draw command array
    m_drawCmds.Clear();

    SetDepth(0.0f);

    // reset flags?
}

void UI::DrawNow()
{
    // Update buffers
    cvar vbSize = m_vertexCount * m_vertexSize;
    cvar ibSize = m_indexCount * m_indexSize;

    cvar vbData = Renderer::Allocate(vbSize, 1);
    cvar ibData = Renderer::Allocate(ibSize, 1);

    memcpy(vbData, m_vertexBufferData, vbSize);
    memcpy(ibData, m_indexBufferData, ibSize);

    // Update buffers
    Renderer::UpdateVertexBuffer(m_vertexBuffer, vbData, m_vertexBufferDataPos, 0u);
    Renderer::UpdateIndexBuffer(m_indexBuffer, ibData, m_indexBufferDataPos, 0u);

    // apply buffers
    Renderer::ApplyVertexBuffer(m_vertexBuffer);
    Renderer::ApplyIndexBuffer(m_indexBuffer);

    // draw
    Renderer::ApplyShader(m_shader->m_shaderHandle, 0);
    Renderer::DrawIndexed(m_indexCount);

    // reset buffer IO positions
    m_vertexBufferDataPos = 0u;
    m_indexBufferDataPos = 0u;
    m_vertexCount = 0u;
    m_indexCount = 0;
}

// protip: we can use forceinline on this method because we are using it only in this source file.
void UI::PushDrawCmd(drawcmd* cmd, int index)
{
    // push draw cmd data

    cmd->indices[0] += index;
    cmd->indices[1] += index;
    cmd->indices[2] += index;
    cmd->indices[3] += index;
    cmd->indices[4] += index;
    cmd->indices[5] += index;

    // copy vertex data
    var vertexBufferData = static_cast<byte*>(m_vertexBufferData);
    vertexBufferData += m_vertexBufferDataPos;

    memcpy(vertexBufferData, cmd->vertices, m_vertexSize * 4);

    // copy index data
    var indexBufferData = static_cast<byte*>(m_indexBufferData);
    indexBufferData += m_indexBufferDataPos;

    memcpy(indexBufferData, cmd->indices, m_indexSize * 6);

    // increase data pos
    m_vertexBufferDataPos += m_vertexSize * 4;
    m_indexBufferDataPos += m_indexSize * 6;

    m_vertexCount += 4;
    m_indexCount += 6;
}

void UI::OnInit()
{
    // create dynamic vertex buffer
    m_vertexBufferData = Renderer::Allocate(m_vertexBufferSize, 0);
    m_vertexBuffer = Renderer::CreateVertexBuffer(m_maxVertexCount, m_vertexSize, true);

    // create dynamic index buffer
    m_indexBufferData = Renderer::Allocate(m_indexBufferSize, 0);
    m_indexBuffer = Renderer::CreateIndexBuffer(m_maxIndexCount, false, true);

    // load shader
    m_shader = Shader::LoadShader("../assets/shaders/UIDefault.shader");

    // allocate draw command for first upload (it's 1/4 of max vertex count as there is 4 vertexes per command)
    m_drawCmds = Array<drawcmd>();
}

void UI::OnDispose()
{
    // Destroy buffers
    Renderer::DestroyVertexBuffer(m_vertexBuffer);
    Renderer::DestroyIndexBuffer(m_indexBuffer);

    // free buffer data
    Renderer::Free(m_vertexBufferData);
    Renderer::Free(m_indexBufferData);

    // Dispose shader
    SafeDispose(m_shader);

    // Release draw cmd array
    m_drawCmds.Release();
}

void UI::BeginDraw()
{
    // clear before drawing
    Clear();
}

void UI::EndDraw()
{
    // sort using zOrder (slave key) and texture (master key)
    sort(m_drawCmds.begin(), m_drawCmds.end(), drawcmd_comparison);

    cvar drawCmdCount = m_drawCmds.Size();

    var vertexCount = 0;
    Renderer::Texture2DHandle textureHandle;
    textureHandle.idx = 0xFFFFFFFF;

    for (var i = 0u; i < m_drawCmds.Size(); i++)
    {
        cvar drawcmd = &m_drawCmds[i];
        cvar textureChanged = drawcmd->texture > 0 && textureHandle.idx != drawcmd->texture;

        if (vertexCount + 4u > m_maxVertexCount || (textureChanged && vertexCount > 0))
        {
            // draw now, reset, and draw more!
            DrawNow();
            vertexCount = 0;
        }

        // set new texture
        if (textureChanged)
        {
           textureHandle.idx = drawcmd->texture;
           Renderer::ApplyTexture2D(textureHandle, 0u);
        }

        // push draw command
        PushDrawCmd(drawcmd, vertexCount);

        // increase the vertex count used for batching
        vertexCount += 4;

        // draw if this is the last command
        if (i + 1 >= drawCmdCount)
        {
            DrawNow();
            break;
        }
    }

    // clear after drawing
    Clear();
}

void UI::SetColor(Color color)
{
    m_instance->m_color = color;
    m_instance->m_color_r = color.r / 255.0f;
    m_instance->m_color_g = color.g / 255.0f;
    m_instance->m_color_b = color.b / 255.0f;
    m_instance->m_color_a = color.a / 255.0f;
}

Color UI::GetColor()
{
    return m_instance->m_color;
}

void UI::SetDepth(float depth)
{
    m_instance->m_depth = depth;
}

float UI::GetDepth()
{
    return m_instance->m_depth;
}

void UI::SetViewRect(Rectf* viewRect, bool enabled)
{
    m_instance->m_viewRect = *viewRect;
    m_instance->m_useViewRect = enabled;
}
