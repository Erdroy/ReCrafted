// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef FONT_H
#define FONT_H

// includes
#include "Core/Types.h"
#include "Core/Math/Rect.h"
#include "Core/Math/Rectf.h"
#include "Common/Text.h"
#include "Texture.h"

#include <vector>
#include <Scripting/Object.h>

/**
 * \brief Font class.
 */
class Font : public Object
{
    friend class UI;

private:
SCRIPTING_API_IMPL()

public:
    struct Glyph
    {
        Font* font;

        Char character = 0;
        uint texture = 0;
        Rect rectangle = {};
        uint advanceY = 0;
        uint advanceX = 0;
        uint horizontalBearingX = 0;
        uint horizontalBearingY = 0;
    };

private:
    uint m_size = 0;
    uint m_charmapWidth = 512;
    uint m_charmapHeight = 512;

    Array<Texture*> m_textures = {};

    Glyph* m_glyphs = nullptr;
    Glyph m_nullGlyph = {};
    uint m_glyphCount = 0;

    float m_lineHeigh = 1.0f; // actually multiplier

private:
    void OnDestroy() override
    {
        if (m_glyphs)
            delete[] m_glyphs;
    }

public:
    /**
	 * \brief Initializez font.
	 * \param glyphCount The total glyph count.
	 */
    void Init(uint glyphCount)
    {
        m_glyphCount = glyphCount;
        m_glyphs = new Glyph[glyphCount];
        memset(m_glyphs, 0, glyphCount * sizeof Glyph);
    }

    /**
	 * \brief Gets character glyph info struct.
	 * \param character The character.
	 * \return The character's glyph info.
	 */
    FORCEINLINE Glyph GetCharacter(Char character) const
    {
        return m_glyphs[character];
    }

    /**
	 * \brief Measures text size.
	 * \param text The text to be measured.
	 * \return The measured size.
	 */
    Vector2 MeasureText(Text& text);

    /**
	 * \brief Loads font from file and renders it into given size.
	 * \param fontFile The font file name.
	 * \param size The size of the font.
	 * \param managed
	 */
    void LoadFont(Text& fontFile, int size, bool managed = false);

    /**
     * \brief Gets texture of this font of given id.
     * \param id The texture id.
     * \return The texture.
     */
    FORCEINLINE Texture* GetTexture(int id) const
    {
        return m_textures[id];
    }

    /**
     * \brief The size of this font.
     */
    FORCEINLINE uint GetSize() const
    {
        return m_size;
    }

    /**
    * \brief The line height multiplier of this font.
    */
    FORCEINLINE float GetLineHeight() const
    {
        return m_lineHeigh;
    }

public:
    virtual const char* GetObjectName() const override
    {
        return "Font";
    }
};

#endif // FONT_H
