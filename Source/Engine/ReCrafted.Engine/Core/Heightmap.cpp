// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Heightmap.h"
#include "Common/IO/Stream.h"
#include "Common/IO/BinaryStream.h"

inline uint8_t FetchBitmapTexelScalar(const Heightmap::HeightmapBitmap& bitmap, const int& x, const int& y)
{
    const auto offset = x + y * bitmap.RowPitch;
    return static_cast<uint8_t*>(bitmap.Data)[offset];
}

void Heightmap::FlatFromStream(const uint16_t width, const uint16_t height, const uint16_t lodCount, BinaryStream* stream)
{
    _ASSERT_(false, "Only HeightmapFormat::Cubical_8bit is currently supported!");
    // TODO: Flat heightmap support
}

void Heightmap::CubicalFromStream(const uint16_t width, const uint16_t height, const uint16_t lodCount, BinaryStream* stream)
{
    _ASSERT_(m_format == HeightmapFormat::Cubical_8bit, "Only HeightmapFormat::Cubical_8bit is currently supported!");

    const auto numCubeFaces = 6;

    // Get the texel size (in bytes) from the format
    const auto texelSize = m_format == HeightmapFormat::Cubical_8bit ? 1 : 2;

    // Go around `lodCount` mip chains, with 6 bitmaps each.
    auto mipWidth = width;
    auto mipHeight = height;
    for(auto mipId = 0; mipId < lodCount; mipId ++)
    {
        HeightmapCubeMip mip;
        mip.MipId = mipId;

        for (auto faceId = 0; faceId < numCubeFaces; faceId++)
        {
            HeightmapBitmap face;
            face.FaceId = faceId;

            // Get the bitmap size 
            const auto bitmapSize = mipWidth * mipHeight * texelSize;

            // Setup data
            face.DataSize = bitmapSize;
            face.RowPitch = mipWidth * texelSize;
            face.Width = mipWidth;
            face.Height = mipHeight;
            face.Data = new uint8_t[bitmapSize];

            // Get data pointer and read data.
            stream->ReadBytes(reinterpret_cast<char*>(face.Data), bitmapSize);

            // Add face
            mip.Faces.Add(face);
        }

        m_cubeMips.Add(mip);

        mipWidth /= 2;
        mipHeight /= 2;
    }
}

Heightmap::~Heightmap()
{
    for (auto mip : m_flatMips)
    {
        delete[] mip.Data;
    }

    for (auto mip : m_cubeMips)
    {
        for (auto face : mip.Faces)
        {
            delete[] face.Data;
        }
    }
}

void Heightmap::InitializeFromMemory(const uint16_t width, const uint16_t height, const uint16_t lodCount, uint8_t* data)
{
    const auto numCubeFaces = 6;

    // Note: Mips are going from the largest to the smallest.

    // Get the texel size (in bytes) from the format
    const auto texelSize = m_format == HeightmapFormat::Cubical_8bit || m_format == HeightmapFormat::Flat_8bit ? 1 : 2;
    
    // Calculate total heightmap size (in bytes)
    auto heightmapSize = 0;
    auto mipWidth = width;
    auto mipHeight = height;
    
    for (auto mip = 0; mip < lodCount; mip++)
    {
        if (m_format == HeightmapFormat::Cubical_8bit || m_format == HeightmapFormat::Cubical_16bit)
        {
            // Get and add the six times the bitmap size
            heightmapSize += mipWidth * mipHeight * texelSize * numCubeFaces;
        }
        else 
        {
            // Get and add the bitmap size 
            heightmapSize += mipWidth * mipHeight * texelSize;
        }

        mipWidth /= 2;
        mipHeight /= 2;
    }

    // Create stream
    auto stream = Stream(static_cast<void*>(data), heightmapSize);
    auto binaryStream = BinaryStream(stream);

    switch (m_format) {
    case HeightmapFormat::Flat_8bit:
    case HeightmapFormat::Flat_16bit:
        FlatFromStream(width, height, lodCount, &binaryStream);
        break;
    case HeightmapFormat::Cubical_8bit:
    case HeightmapFormat::Cubical_16bit:
        CubicalFromStream(width, height, lodCount, &binaryStream);
        break;

    case HeightmapFormat::Unknown:
    case HeightmapFormat::Count:
    default:
        _ASSERT_(false, "Unknown heightmap format");
    }
}

void Heightmap::InitializeFromStream(const uint16_t width, const uint16_t height, const uint16_t lodCount, BinaryStream* stream)
{
    switch (m_format) {
    case HeightmapFormat::Flat_8bit:
    case HeightmapFormat::Flat_16bit:
        FlatFromStream(width, height, lodCount, stream);
        break;
    case HeightmapFormat::Cubical_8bit:
    case HeightmapFormat::Cubical_16bit:
        CubicalFromStream(width, height, lodCount, stream);
        break;

    case HeightmapFormat::Unknown:
    case HeightmapFormat::Count:
    default:
        _ASSERT_(false, "Unknown heightmap format");
    }
}

uint8_t Heightmap::SampleFlat8BitPoint(Vector2 uv, uint8_t lod)
{
    DEBUG_ASSERT(m_format == HeightmapFormat::Flat_8bit);
    _ASSERT_(false, "SampleFlat8BitPoint is not currently implemented!");
    return 0u;
}

uint16_t Heightmap::SampleFlat16BitPoint(Vector2 uv, uint8_t lod)
{
    DEBUG_ASSERT(m_format == HeightmapFormat::Flat_16bit);
    _ASSERT_(false, "SampleFlat16BitPoint is not currently implemented!");
    return 0u;
}

uint8_t Heightmap::SampleFlat8BitBilinear(Vector2 uv, uint8_t lod)
{
    DEBUG_ASSERT(m_format == HeightmapFormat::Flat_8bit);
    _ASSERT_(false, "SampleFlat8BitBilinear is not currently implemented!");
    return 0u;
}

uint16_t Heightmap::SampleFlat16BitBilinear(Vector2 uv, uint8_t lod)
{
    DEBUG_ASSERT(m_format == HeightmapFormat::Flat_16bit);
    _ASSERT_(false, "SampleFlat16BitBilinear is not currently implemented!");
    return 0u;
}

uint8_t Heightmap::SampleCube8BitPoint(CubeFace baseFace, Vector2 uv, uint8_t lod)
{
    DEBUG_ASSERT(m_format == HeightmapFormat::Cubical_8bit);
    _ASSERT_(false, "SampleCube8BitPoint is not currently implemented!");
    return 0u;
}

uint16_t Heightmap::SampleCube16BitPoint(CubeFace baseFace, Vector2 uv, uint8_t lod)
{
    DEBUG_ASSERT(m_format == HeightmapFormat::Cubical_16bit);
    _ASSERT_(false, "SampleCube16BitPoint is not currently implemented!");
    return 0u;
}

uint8_t Heightmap::SampleCube8BitBilinear(const CubeFace baseFace, const Vector2 uv, const uint8_t lod)
{
    DEBUG_ASSERT(m_format == HeightmapFormat::Cubical_8bit);
    DEBUG_ASSERT(m_cubeMips.Size() > 0);
    DEBUG_ASSERT(uv.x >= 0.0f && uv.x <= 1.0f && uv.y >= 0.0f && uv.y <= 1.0f);

    const auto mip = m_cubeMips[lod];
    DEBUG_ASSERT(mip.Faces.Size() > 0);
    const auto bitmap = mip.Faces[baseFace];

    const auto u = uv.x;
    const auto v = uv.y;

    const auto px = uint16_t(u * float(bitmap.Width));
    const auto py = uint16_t(v * float(bitmap.Height));

    const auto needFlipX = px + 1 >= bitmap.Width; // x + 1 == Width: sample mip going right
    const auto needFlipY = py + 1 >= bitmap.Height; // y + 1 == Height: sample mip going down (warning: we need to properly swap axis!)

    uint8_t texel0;
    uint8_t texel1;
    uint8_t texel2;
    uint8_t texel3;

    // Fetch texels
    if (!needFlipX && !needFlipY)
    {
        // Sample simple
        texel0 = FetchBitmapTexelScalar(bitmap, px, py);
        texel1 = FetchBitmapTexelScalar(bitmap, px + 1, py);
        texel2 = FetchBitmapTexelScalar(bitmap, px, py + 1);
        texel3 = FetchBitmapTexelScalar(bitmap, px + 1, py + 1);
    }
    else
    {
        // Note: We're loosing some precision right there, but it is totally unnoticeable
        // as far as the cubical height map is done correctly. This is a really good trade-off
        // as we don't have to flip and sample different cube faces, so basically we're saving
        // a lot of branching and performance right there.

        if (needFlipX && !needFlipY)
        {
            texel0 = texel1 = FetchBitmapTexelScalar(bitmap, px - 1, py);
            texel2 = texel3 = FetchBitmapTexelScalar(bitmap, px - 1, py + 1);
        }
        else if (!needFlipX && needFlipY)
        {
            texel0 = texel1 = FetchBitmapTexelScalar(bitmap, px, py - 1);
            texel2 = texel3 = FetchBitmapTexelScalar(bitmap, px + 1, py - 1);
        }
        else
        {
            texel0 = texel1 = FetchBitmapTexelScalar(bitmap, px - 1, py - 1);
            texel2 = texel3 = FetchBitmapTexelScalar(bitmap, px - 2, py - 2);
        }
    }

    const auto x = u * float(bitmap.Width);
    const auto y = v * float(bitmap.Height);

    const auto fx = x - float(px);
    const auto fy = y - float(py);
    const auto fx1 = 1.0f - fx;
    const auto fy1 = 1.0f - fy;

    // Calculate weights
    const auto w1 = float(fx1 * fy1 * 256.0f);
    const auto w2 = float(fx * fy1 * 256.0f);
    const auto w3 = float(fx1 * fy * 256.0f);
    const auto w4 = float(fx * fy * 256.0f);

    // Calculate and return
    const auto out = int(float(texel0) * w1 + float(texel1) * w2 + float(texel2) * w3 + float(texel3) * w4);

    return uint8_t(out >> 8);
}

uint16_t Heightmap::SampleCube16BitBilinear(CubeFace baseFace, Vector2 uv, uint8_t lod)
{
    DEBUG_ASSERT(m_format == HeightmapFormat::Cubical_16bit);
    _ASSERT_(false, "SampleCube16BitBilinear is not currently implemented!");
    return 0u;
}
