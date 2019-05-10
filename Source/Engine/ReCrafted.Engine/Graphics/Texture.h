// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/IO/FileStream.h"
#include "Core/Threading/Task.h"
#include "Content/Assets/BinaryAsset.h"
#include "Renderer/Renderer.h"
#include "Scripting/Object.h"

class BinaryStream;

API_USING("ReCrafted.API.Content")

API_CLASS(public, sealed)
class Texture final : public BinaryAsset
{
    API_CLASS_BODY()

public:
    static const size_t TextureHeaderSize = sizeof(uint32_t) + sizeof(uint16_t) + sizeof(uint8_t);
    static const size_t MipHeaderSize = sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint16_t) + sizeof(int32_t);

private:
    struct LoadTextureMipTask : ITask
    {
    public:
        Texture* texture = nullptr;
        int mipLevel = 0;
        int mipCount = 0;
        File file = File{ nullptr, 0u };

        Renderer::RendererMemory m_mipData = nullptr;
        size_t m_mipDataSize = 0u;

    public:
        void Execute(void* userData) override;
        void Finish() override;
    };

    struct TextureInfo
    {
    public:
        size_t size = 0u;
        uint16_t format = 0u;
        uint8_t mips = 0u;

    public:
        static TextureInfo ReadTextureInfo(BinaryStream& stream);
    };

    struct MipInfo
    {
    public:
        uint16_t width = 0u;
        uint16_t height = 0u;
        uint16_t pitch = 0u;
        size_t size = 0u;

    public:
        static MipInfo ReadMipInfo(BinaryStream& stream);
    };

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

    bool m_lazyLoading = false;
    bool m_applied = false;

private:
    size_t CalculateMipSize(uint8_t mipId) const;
    size_t CalculateMipOffset(uint8_t mipId) const;

    void LoadTexture(BinaryStream& stream);
    void LazyLoadTexture(const TextureInfo& textureInfo, File& file, BinaryStream& stream);

protected:
    void OnInitialize() override;
    void OnLoadBegin(const std::string& fileName) override;
    void OnDeserializeBinary(uint16_t version, BinaryStream& stream) override;
    void OnUnload() override;

    AssetType GetAssetType() override
    {
        return AssetType::Texture2D;
    }

    bool CanLoadAsync() override
    {
        return false; // False, because texture has it's own task-based lazy loading
    }

    bool IsLoaded() const override
    {
        return (m_loaded && !m_lazyLoading) || IsVirtual();
    }

public:
    /**
     * \brief Initializes this texture and allocates memory.
     * \param width The texture width.
     * \param height The texture height.
     */
    void Initialize(uint16_t width, uint16_t height, Renderer::TextureFormat::_enum format = Renderer::TextureFormat::RGBA8);

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

    /**
     * \brief Resizes the texture to given size.
     * \param width The new texture width.
     * \param height The new texture height.
     *
     * \note - Only texture created as Virtual Asset with Initialize function can be resized.
     * \note - Mip-maps are not supported.
     */
    void Resize(uint16_t width, uint16_t height);

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
     * \brief Check if this texture is 3D or 2D.
     * \return True, when this texture is 3D texture.
     */
    bool Is3D() const
    {
        return false;
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
