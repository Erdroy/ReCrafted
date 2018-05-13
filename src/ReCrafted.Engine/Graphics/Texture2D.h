// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef TEXTURE2D_H
#define TEXTURE2D_H

// includes
#include "ReCrafted.h"
#include "Core/Types.h"
#include "Common/ReCraftedAPI.h"
#include "Scripting/Object.h"
#include "Renderer/Renderer.hpp"
#include "IResource.h"

typedef struct upng_t upng_t;

/**
 * \brief Texture2D class.
 */
class Texture2D : public Object, IResource
{
    friend class Shader;

private:
SCRIPTING_API_IMPL()

private:
    Renderer::Texture2DHandle m_textureHandle;

    upng_t* m_bitmap = nullptr;
    byte* m_bits = nullptr;

    uint m_flags = 0u;
    uint m_width = 0u;
    uint m_height = 0u;
    uint m_bpp = 0u;
    uint m_mips = 0u;

    Renderer::TextureFormat::_enum m_textureFormat = Renderer::TextureFormat::RGBA8;

public:
IRESOURCE_IMPL(Texture2D)

private:
    void onDestroy() override;

    static void releaseBitmap(void* ptr, void* userData);
    static void releaseData(void* ptr, void* userData);

public:
    void dispose() override;

public:
    /**
	 * \brief Loads texture from file.
	 * \param filename The file.
	 * \param flags BGFX Texture flags.
	 */
    void loadFile(const char* filename, uint flags = 0u);

    /**
	 * \brief Loads texture from memory.
	 * \param data The texture bits pointer.
	 * \param width The width.
	 * \param height The height.
	 * \param flags BGFX Texture flags.
	 */
    void loadMemory(void* data, int width, int height, uint flags = 0u);

    /**
	 * \brief Creates texture memory of given size.
	 * \param width The width.
	 * \param height The height.
	 * \param flags BGFX Texture flags.
	 */
    void createMemory(int width, int height, uint flags = 0u);

    /**
	 * \brief Get pixel at given position.
	 * \param x The x coord.
	 * \param y The y coord.
	 * \return Pixel HEX format.
	 */
    uint getPixel(int x, int y);

    /**
	 * \brief Get all pixels in the texture(Returns pixel buffer).
	 * \return The pixels data.
	 * 
	 * \note This will be invalid after calling `apply`.
	 */
    uint* getPixels() const;

    /**
	 * \brief Sets pixel at given position.
	 * \param x The x coord.
	 * \param y The y coord.
	 * \param pixel Pixel in HEX format.
	 */
    void setPixel(int x, int y, uint pixel);

    /**
    * \brief Sets pixel at given position.
    * \param x The x coord.
    * \param y The y coord.
    * \param r R channel value.
    * \param g G channel value.
    * \param b B channel value.
    * \param a A channel value.
    */
    void setPixel(int x, int y, byte r, byte g, byte b, byte a);

    /**
	 * \brief Sets pixels at given position with given size.
	 * \param x The start X coord.
	 * \param y The start Y coord.
	 * \param width The width of the pixel array.
	 * \param height The height of the pixel array. 
	 * \param pixels The pixels data.
	 */
    void setPixels(int x, int y, int width, int height, uint* pixels);

    /**
	 * \brief Gets the width of this texture.
	 * \return The width of this texture.
	 */
    uint getWidth() const;

    /**
    * \brief Gets the height of this texture.
    * \return The height of this texture.
    */
    uint getHeight() const;

    /**
	 * \brief Apply, all pixel operations will be unavailable after calling this method.
	 */
    void apply();

public:
    Renderer::Texture2DHandle getHandle() const
    {
        return m_textureHandle;
    }

public:
    /**
	 * \brief Creates new empty texture.
	 * \return The newly created texture.
	 */
    static Ref<Texture2D> createTexture(Renderer::TextureFormat::_enum format);
};

#endif // TEXTURE2D_H
