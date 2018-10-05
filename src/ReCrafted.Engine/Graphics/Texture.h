// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

// includes
#include "ReCrafted.h"
#include "Content/Assets/BinaryAsset.h"
#include "Renderer/Renderer.hpp"
#include "Scripting/Object.h"

class Texture : public BinaryAsset
{
private:
    SCRIPTING_API_IMPL()

private:
    Renderer::Texture2DHandle m_textureHandle;
    Renderer::RendererMemory m_textureMemory = nullptr;
    size_t m_textureMemorySize = 0u;
    Renderer::TextureFormat::_enum m_textureFormat = Renderer::TextureFormat::RGBA8;

    uint16_t m_width = 0u;
    uint16_t m_height = 0u;
    uint16_t m_format = 0u;
    uint16_t m_pitch = 0u;
    uint8_t m_mips = 0u;

    bool m_applied = false;

protected:
    void OnInitialize() override;
    void OnLoadBegin(const std::string& fileName) override;
    void OnDeserializeBinary(uint16_t version, BinaryStream& stream) override;
    void OnUnload() override;
    
    AssetType GetAssetType() override
    {
        return AssetType::Texture2D;
    }

public:
    /**
     * \brief Initializes this texture and allocates memory using texture format RGBA8.
     * \param width The texture width.
     * \param height The texture height.
     */
    void Initialize(uint16_t width, uint16_t height);

    /**
     * \brief Initializes this texture from given memory pointer. Data gets copied into allocated texture memory.
     * \param data The data pointer.
     * \param dataSize The data size.
     * \param width The texture width.
     * \param height The texture height.
     * \param mipNum The amount of mip maps.
     * \param textureFormat The texture format.
     * 
     * \note Requires Apply call.
     */
    void InitializeFromMemory(uint8_t* data, size_t dataSize, uint16_t width, uint16_t height, uint8_t mipNum,
                              Renderer::TextureFormat::_enum textureFormat);

    /**
     * \brief Initializes this texture from given file.
     * \param fileName The file which will be loaded as texture.
     * 
     * \note Only PNG RGBA8 format is supported! 
     * Requires Apply call.
     */
    void InitializeFromFile(const char* fileName);

    /**
     * \brief Get pixel at given position.
     * \param x The x coord.
     * \param y The y coord.
     * \return Pixel HEX format.
     * 
     * \note Available only before Apply call, when format is RGBA8 
     * and has been initialized (Texture::Initialize(...)).
     */
    uint GetPixel(int x, int y) const;

    /**
     * \brief Get all pixels in the texture(Returns pixel buffer).
     * \return The pixels data.
     *
     * \note Available only before Apply call.
     */
    uint* GetPixels() const;

    /**
     * \brief Sets pixel at given position.
     * \param x The x coord.
     * \param y The y coord.
     * \param pixel Pixel in HEX format.
     * 
     * \note Available only before Apply call, when format is RGBA8 
     * and has been initialized (Texture::Initialize(...)).
     */
    void SetPixel(int x, int y, uint pixel) const;

    /**
     * \brief Sets pixel at given position.
     * \param x The x coord.
     * \param y The y coord.
     * \param r R channel value.
     * \param g G channel value.
     * \param b B channel value.
     * \param a A channel value.
     *
     * \note Available only before Apply call, when format is RGBA8
     * and has been initialized (Texture::Initialize(...)).
     */
    void SetPixel(int x, int y, byte r, byte g, byte b, byte a) const;

    /**
     * \brief Sets pixels at given position with given size.
     * \param x The start X coord.
     * \param y The start Y coord.
     * \param width The width of the pixel array.
     * \param height The height of the pixel array.
     * \param pixels The pixels data.
     * 
     * \note Available only before Apply call.
     */
    void SetPixels(int x, int y, int width, int height, uint* pixels) const;

    /**
     * \brief Apply, all pixel operations will be unavailable after calling this method.
     * \param generateMips When true, this texture will get online-generated mip maps.
     */
    void Apply(bool generateMips = false);

public:
    /**
     * \brief Gets the width of this texture.
     * \return The width of this texture.
     */
    uint GetWidth() const
    {
        return m_width;
    }

    /**
    * \brief Gets the height of this texture.
    * \return The height of this texture.
    */
    uint GetHeight() const
    {
        return m_height;
    }

    /**
     * \brief Gets handle of this texture.
     * \return G
     */
    Renderer::Texture2DHandle GetHandle() const
    {
        ASSERT(RENDERER_CHECK_HANDLE(m_textureHandle));
        return m_textureHandle;
    }
};

#endif // TEXTURE_H
