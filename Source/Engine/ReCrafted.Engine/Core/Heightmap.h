// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/List.h"
#include "Core/HeightmapFormat.h"

/// <summary>
///     Heightmap class. Provides heightmap sampling functions.
///     
///     A typical flat heightmap consists of singe mipmap chain of x bitmaps, 
///     each two times smaller than the previous.
///     
///     A typical cubical heightmap consists of six mipmap chains of x bitmaps, 
///     each two times smaller than the previous.
/// </summary>
class Heightmap final
{
    DELETE_COPY_MOVE(Heightmap);

public:
    /// <summary>
    ///     Contains all face types of a typical cube.
    /// </summary>
    enum CubeFace : int
    {
        Front,
        Back,
        Left,
        Right,
        Top,
        Bottom,

        Count
    };

    struct HeightmapBitmap
    {
        int FaceId = -1;
        size_t DataSize = 0u;
        size_t RowPitch = 0u;
        uint16_t Width = 0u;
        uint16_t Height = 0u;
        uint8_t* Data = nullptr;
    };

    struct HeightmapCubeMip
    {
        int MipId = -1;
        List<HeightmapBitmap> Faces = {};
    };

private:
    HeightmapFormat m_format = HeightmapFormat::Unknown;

    List<HeightmapBitmap> m_flatMips = {};
    List<HeightmapCubeMip> m_cubeMips = {};

private:
    void FlatFromStream(uint16_t width, uint16_t height, uint16_t lodCount, BinaryStream* stream);
    void CubicalFromStream(uint16_t width, uint16_t height, uint16_t lodCount, BinaryStream* stream);

public:
    Heightmap() : m_format(HeightmapFormat::Flat_8bit) {}
    explicit Heightmap(const HeightmapFormat format) : m_format(format) {}
    ~Heightmap();

public:
    void InitializeFromMemory(uint16_t width, uint16_t height, uint16_t lodCount, uint8_t* data);
    void InitializeFromStream(uint16_t width, uint16_t height, uint16_t lodCount, BinaryStream* stream);

public:
    uint8_t SampleFlat8BitPoint(Vector2 uv, uint8_t lod);
    uint16_t SampleFlat16BitPoint(Vector2 uv, uint8_t lod);
    uint8_t SampleFlat8BitBilinear(Vector2 uv, uint8_t lod);
    uint16_t SampleFlat16BitBilinear(Vector2 uv, uint8_t lod);

    uint8_t SampleCube8BitPoint(CubeFace baseFace, Vector2 uv, uint8_t lod);
    uint16_t SampleCube16BitPoint(CubeFace baseFace, Vector2 uv, uint8_t lod);
    uint8_t SampleCube8BitBilinear(CubeFace baseFace, Vector2 uv, uint8_t lod);
    uint16_t SampleCube16BitBilinear(CubeFace baseFace, Vector2 uv, uint8_t lod);
};