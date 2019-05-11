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

/// <summary>
///     Texture asset class.
/// </summary>
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
    /// <summary>
    ///     Initializes this texture and allocates memory.
    /// </summary>
    /// <param name="width">The texture width.</param>
    /// <param name="height">The texture height.</param>
    /// <param name="format">The texture format.</param>
    void Initialize(uint16_t width, uint16_t height, Renderer::TextureFormat::_enum format);

    /// <summary>
    ///     Initializes this texture from given memory pointer. Data gets copied into allocated texture memory.
    /// </summary>
    /// <param name="data">The data pointer.</param>
    /// <param name="dataSize">The data size.</param>
    /// <param name="width">The texture width.</param>
    /// <param name="height">The texture height.</param>
    /// <param name="mipNum">The amount of mip maps.</param>
    /// <param name="format">The texture format.</param>
    /// <remarks>Requires Apply call.</remarks>
    void InitializeFromMemory(uint8_t* data, size_t dataSize, uint16_t width, uint16_t height, uint8_t mipNum, Renderer::TextureFormat::_enum format);

    /// <summary>
    ///     Initializes this texture from given file.
    /// </summary>
    /// <param name="fileName">The file which will be loaded as texture.</param>
    /// <remarks>Only PNG RGBA8 format is supported! Requires Apply call.</remarks>
    API_FUNCTION()
    void InitializeFromFile(const char* fileName);

    /// <summary>
    ///     Get pixel at given position.
    /// </summary>
    /// <param name="x">The start X coord.</param>
    /// <param name="y">The start Y coord.</param>
    /// <remarks>Available only before Apply call, when format is RGBA8 and has been initialized(Texture::Initialize(...)).</remarks>
    API_FUNCTION()
    uint GetPixel(int x, int y) const;

    /// <summary>
    ///     Get all pixels in the texture(Returns pixel buffer).
    /// </summary>
    /// <returns>The pixels data.</returns>
    /// <remarks>Available only before Apply call.</remarks>
    /// TODO: API return list support
    uint* GetPixels() const;

    /// <summary>
    ///     Sets pixel at given position.
    /// </summary>
    /// <param name="x">The start X coord.</param>
    /// <param name="y">The start Y coord.</param>
    /// <param name="pixel">Pixel in HEX format.</param>
    /// <remarks>Available only before Apply call, when format is RGBA8 and has been initialized(Texture::Initialize(...)).</remarks>
    API_FUNCTION()
    void SetPixel(int x, int y, uint pixel) const;

    /// <summary>
    ///     Sets pixel at given position.
    /// </summary>
    /// <param name="x">The start X coord.</param>
    /// <param name="y">The start Y coord.</param>
    /// <param name="r">R channel value.</param>
    /// <param name="g">G channel value.</param>
    /// <param name="b">B channel value.</param>
    /// <param name="a">A channel value.</param>
    /// <remarks>Available only before Apply call, when format is RGBA8 and has been initialized(Texture::Initialize(...)).</remarks>
    API_FUNCTION()
    void SetPixel(int x, int y, byte r, byte g, byte b, byte a) const;

    /// <summary>
    ///     Sets pixels at given position with given size.
    /// </summary>
    /// <param name="x">The start X coord.</param>
    /// <param name="y">The start Y coord.</param>
    /// <param name="width">The width of the pixel array.</param>
    /// <param name="height">The height of the pixel array.</param>
    /// <param name="pixels">The pixels data.</param>
    /// <remarks>Available only before Apply call.</remarks>
    void SetPixels(int x, int y, int width, int height, uint* pixels) const;

    /// <summary>
    ///     Apply, all pixel operations will be unavailable after calling this method.
    /// </summary>
    /// <param name="generateMips">When true, this texture will get online-generated mip maps.</param>
    void Apply(bool generateMips = false);

    /// <summary>
    ///     Resizes the texture to given size.
    /// </summary>
    /// <param name="width">The new texture width.</param>
    /// <param name="height">The new texture height.</param>
    /// <remarks>Only texture created as Virtual Asset with Initialize function can be resized.</remarks>
    /// <remarks>Mip-maps are not supported.</remarks>
    API_FUNCTION()
    void Resize(uint16_t width, uint16_t height);

public:
    /// <summary>
    ///     Gets the width of this texture.
    /// </summary>
    API_PROPERTY(noprefix)
    uint GetWidth() const
    {
        return m_width;
    }

    /// <summary>
    ///     Gets the height of this texture.
    /// </summary>
    API_PROPERTY(noprefix)
    uint GetHeight() const
    {
        return m_height;
    }

    /// <summary>
    ///     Gets the texture 3D state. True, when this texture is 3D texture false when 2D.
    /// </summary>
    API_PROPERTY()
    bool Is3D() const
    {
        return false;
    }

    /// <summary>
    ///     Gets handle of this texture.
    /// </summary>
    /// <returns>The texture handle.</returns>
    Renderer::Texture2DHandle GetHandle() const
    {
        ASSERT(RENDERER_CHECK_HANDLE(m_textureHandle));
        return m_textureHandle;
    }
};
