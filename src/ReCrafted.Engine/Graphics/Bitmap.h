// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef BITMAP_H
#define BITMAP_H

// includes
#include "ReCrafted.h"
#include "Platform/Platform.h"
#include "IResource.h"

#pragma pack(push, 1)
struct BitmapHeader
{
public:
    char signature[2];
    uint32_t size;
    uint16_t reserved_a;
    uint16_t reserved_b;
    uint32_t bitmap_offset;
    uint32_t header_size;
    uint32_t width;
    uint32_t height;
    uint16_t plane_count;
    uint16_t bits_per_pixel;
    uint32_t compresion_type;
    uint32_t image_size;
    uint32_t h_res_per_meter;
    uint32_t v_res_per_meter;
    uint32_t color_count;
    uint32_t imp_color_count;
};
#pragma pack(pop)

class Bitmap
{
private:
    Bitmap()
    {
    }

public:
    static byte* Load(const char* fileName, int* width, int* height, int* bpp = nullptr)
    {
        if (!Platform::FileExists(fileName))
        {
            Logger::LogError("Bitmap::load() file '{0}' not found!", fileName);
            return nullptr;
        }

        File bmp_file;
        Platform::OpenFile(&bmp_file, fileName, OpenMode::OpenRead);

        BitmapHeader header;
        bmp_file.Read(&header, sizeof BitmapHeader);

        // write out the width and height
        *width = static_cast<int>(header.width);
        *height = static_cast<int>(header.height);

        if (bpp)
            *bpp = static_cast<int>(header.bits_per_pixel);

        cvar image_size = header.size - header.bitmap_offset;
        cvar bitmap = new byte[image_size];

        // read image data
        bmp_file.Read(bitmap, image_size, header.bitmap_offset);

        return bitmap;
    }

    static void Save(byte* bitmap, int width, int height, int bpp)
    {
        // TODO: save bitmap
    }

    static void Free(byte* bitmap)
    {
        SafeDeleteArray(bitmap);
    }
};

#endif // BITMAP_H
