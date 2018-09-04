// ReCrafted (c) 2016-2018 Always Too Late

#include "UI.h"
#include "Common/Display.h"
#include "Graphics/Font.h"

#define BOX_VERTICES_DEFINE() vertex v0 = {}; vertex v1 = {}; vertex v2 = {}; vertex v3 = {}; Rectf uvdiff = {};

#define BOX_VERTICES_SETUP() SetupVertexData(rect, v0, v1, v2, v3, &uvdiff);

#define BOX_VERTICES_FINALIZE(texture) FinalizeVertexData(v0, v1, v2, v3, texture);

#define BOX_VERTICES_SETUP_UVS() v0.u = uvs.x;\
v0.v = uvs.y + uvs.height;\
v1.u = uvs.x + uvs.width;\
v1.v = uvs.y + uvs.height;\
v2.u = uvs.x + uvs.width;\
v2.v = uvs.y;\
v3.u = uvs.x;\
v3.v = uvs.y;

template <typename T>
FORCEINLINE void drawTextBase(UI* instance, Font* font, T* characters, int characterCount, Vector2 position)
{
    // round position
    position.x = roundf(position.x);
    position.y = roundf(position.y);

    auto currentPosition = position;
    auto lineheight = float(font->GetSize()) * font->GetLineHeight();

    for (auto i = 0; i < characterCount; i++)
    {
        auto character = characters[i];
        auto glyph = font->GetCharacter(character);

        if (glyph.font != font)
        {
            // invalid character!
            glyph = font->GetCharacter(Char('?'));
        }

        auto glyphRect = glyph.rectangle;
        auto texture = font->GetTexture(glyph.texture);

        var atlasWidth = static_cast<float>(texture->GetWidth());
        var atlasHeight = static_cast<float>(texture->GetHeight());

        if (character == ' ') // Space
        {
            cvar glyphData = font->GetCharacter(Char('i'));
            currentPosition += Vector2(float(glyphData.advanceX), 0.0f);
        }
        else if (character == 9) // Tab
        {
            cvar glyphData = font->GetCharacter(Char('i'));
            currentPosition += Vector2(float(glyphData.advanceX) * 3, 0.0f);
        }
        else if (character == '\n' || character == 10 || character == 13) // New line character.
        {
            currentPosition.x = position.x;
            currentPosition.y += lineheight;
        }
        else // A 'normal' character.
        {
            auto currentX = currentPosition.x;
            auto currentY = currentPosition.y;

            currentX += glyph.horizontalBearingX;
            currentY += lineheight;
            currentY -= glyph.horizontalBearingY;

            instance->InternalDrawBoxTextured(
                texture.get(),
                Rectf(currentX, currentY, float(glyphRect.width), float(glyphRect.height)),
                Rectf(glyphRect.x / atlasWidth, (glyphRect.y + glyphRect.height) / atlasHeight,
                      glyphRect.width / atlasWidth, -glyphRect.height / atlasHeight));

            currentPosition += Vector2(float(glyph.advanceX), 0.0f);
        }
    }
}

void UI::SetupVertexData(Rectf& rect, vertex& v0, vertex& v1, vertex& v2, vertex& v3, Rectf* uvDiff) const
{
    cvar screenWidth = Display::GetWidth() * 0.5f;
    cvar screenHeight = Display::GetHeight() * 0.5f;

    var uv_xDiff = 0.0f;
    var uv_yDiff = 0.0f;
    var uv_xzDiff = 0.0f;
    var uv_ywDiff = 0.0f;

    if (m_instance->m_useViewRect)
    {
        // apply view rect

        var& clipRect = m_viewRect;

        if (rect.x >= clipRect.x + clipRect.width || rect.x + rect.width <= clipRect.x || rect.y >= clipRect.y +
            clipRect.height || rect.y + rect.height <= clipRect.y)
            return;

        if (rect.x < clipRect.x && rect.x + rect.width > clipRect.x)
        {
            uv_xDiff = clipRect.x - rect.x;
            rect.x = clipRect.x;
            rect.width -= uv_xDiff;
        }

        if (rect.y < clipRect.y && rect.y + rect.height > clipRect.y)
        {
            uv_yDiff = clipRect.y - rect.y;
            rect.y = clipRect.y;
            rect.height -= uv_yDiff;
        }

        if (rect.x < clipRect.x + clipRect.width && rect.x + rect.width > clipRect.x + clipRect.width)
        {
            uv_xzDiff = rect.x + rect.width - (clipRect.x + clipRect.width);
            rect.width -= uv_xzDiff;
        }

        if (rect.y < clipRect.y + clipRect.height && rect.y + rect.height > clipRect.y + clipRect.height)
        {
            uv_ywDiff = rect.y + rect.height - (clipRect.y + clipRect.height);
            rect.height -= uv_ywDiff;
        }
    }

    *uvDiff = Rectf(uv_xDiff, uv_yDiff, uv_xzDiff, uv_ywDiff);

    cvar rX = rect.x;
    cvar rY = rect.y;
    cvar rW = rect.width;
    cvar rH = rect.height;

    // Width
    cvar width = rW / screenWidth;

    // Height
    cvar height = rH / screenHeight;

    // X (Top-left)
    cvar x = rX / screenWidth - 1.0f;

    // Y (Top-left)
    cvar y = 1.0f - rY / screenHeight - height;

    // calculate x and y of the first vertex.
    v0.x = x;
    v0.y = y + height;

    // calculate x and y of the second vertex.
    v1.x = x + width;
    v1.y = y + height;

    // calculate x and y of the third vertex.
    v2.x = x + width;
    v2.y = y;

    // calculate x and y of the fourth vertex.
    v3.x = x;
    v3.y = y;

    // set colors
    v0.r = m_color_r;
    v0.g = m_color_g;
    v0.b = m_color_b;
    v0.a = m_color_a;

    v1.r = m_color_r;
    v1.g = m_color_g;
    v1.b = m_color_b;
    v1.a = m_color_a;

    v2.r = m_color_r;
    v2.g = m_color_g;
    v2.b = m_color_b;
    v2.a = m_color_a;

    v3.r = m_color_r;
    v3.g = m_color_g;
    v3.b = m_color_b;
    v3.a = m_color_a;
}

void UI::FinalizeVertexData(vertex& v0, vertex& v1, vertex& v2, vertex& v3, uint texture)
{
    // build draw command
    auto cmd = drawcmd();

    if (texture > 0)
    {
        cmd.texture = texture;
        cmd.hasTexture = true;
    }
    else
        cmd.hasTexture = false;

    // set depth
    cmd.zOrder = m_depth;

    // copy vertices
    cmd.vertices[0] = v0;
    cmd.vertices[1] = v1;
    cmd.vertices[2] = v2;
    cmd.vertices[3] = v3;

    // first triangle
    cmd.indices[0] = 0;
    cmd.indices[1] = 1;
    cmd.indices[2] = 2;

    // second triangle
    cmd.indices[3] = 2;
    cmd.indices[4] = 3;
    cmd.indices[5] = 0;

    m_drawCmds.Add(cmd);
}

void UI::InternalDrawBox(Rectf rect)
{
    BOX_VERTICES_DEFINE();
    BOX_VERTICES_SETUP();

    BOX_VERTICES_FINALIZE(0);
}

void UI::InternalDrawBoxTextured(Texture2D* texture, Rectf rect, Rectf uvs)
{
    cvar textureHandle = texture->GetHandle().idx;

    BOX_VERTICES_DEFINE();
    BOX_VERTICES_SETUP();

    if (m_instance->m_useViewRect)
    {
        cvar tex_width = static_cast<float>(texture->GetWidth());
        cvar tex_height = static_cast<float>(texture->GetHeight());

        cvar xdiff = uvdiff.x / tex_width;
        cvar ydiff = uvdiff.y / tex_height;

        var xzdiff = uvdiff.width / tex_width;
        var ywdiff = uvdiff.height / tex_height;

        xzdiff += xdiff;
        ywdiff += ydiff;

        uvs.x += xdiff;
        uvs.width -= xzdiff;

        uvs.y += ydiff;
        uvs.height -= ywdiff;
    }

    BOX_VERTICES_SETUP_UVS();

    BOX_VERTICES_FINALIZE(textureHandle);
}

void UI::DrawBox(Rectf rect)
{
    rect.width -= rect.x;
    rect.height -= rect.y;

    m_instance->InternalDrawBox(rect);
}

void UI::DrawText(Font* font, const Text& text, Vector2 position)
{
    DrawText(font, text.Data(), text.Length(), position);
}

void UI::DrawText(Font* font, const Char* characters, int characterCount, Vector2 position)
{
    drawTextBase(m_instance, font, characters, characterCount, position);
}

void UI::DrawText(Font* font, const char* characters, int characterCount, Vector2 position)
{
    drawTextBase(m_instance, font, characters, characterCount, position);
}

void UI::DrawTexture(Texture2D* texture, Rectf rect, Rectf uvs)
{
    rect.width -= rect.x;
    rect.height -= rect.y;

    uvs.y += uvs.height;
    uvs.height = -uvs.height;

    m_instance->InternalDrawBoxTextured(texture, rect, uvs);
}
