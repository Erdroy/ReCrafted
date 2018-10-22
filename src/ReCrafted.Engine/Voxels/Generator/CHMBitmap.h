// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef CHMTEXTURE_H
#define CHMTEXTURE_H

// includes
#include "ReCrafted.h"
#include "Core/Containers/Array.h"

struct CHMFace
{
    enum _enum : uint8_t
    {
        Front,
        Back,
        Left,
        Right,
        Top,
        Bottom,
    };
};

class CHMBitmap : IDisposable
{
private:
    struct FaceBitmap : IDisposable
    {
        uint16_t Width = 0u;
        uint16_t Height = 0u;
        byte* Bitmap = nullptr;

        void Dispose() override
        {
            SafeDeleteArray(Bitmap);
        }
    };

    struct Face : IDisposable
    {
        Array<FaceBitmap> MipMaps = {};

        void Dispose() override
        {
            for (rvar mip : MipMaps)
                mip.Dispose();

            MipMaps.Clear();
        }
    };

private:
    Array<Face> m_faces = {};

private:
    FORCEINLINE byte FetchBitmapTexelScalar(const FaceBitmap& mip, const int& x, const int& y) const;

public:
    CHMBitmap();

public:
    void Dispose() override;

public:
    /**
     * \brief Samples CHM face bitmap using bilinear interpolation filter. 
     * Origin of the bitmap is Top-Left [0.0, 0.0].
     * \param faceType The face type.
     * \param u The normalized u coord (0.0 - 1.0 (excluding)).
     * \param v The normalized v coord (0.0 - 1.0 (excluding)).
     * \param lod The lod. 0 - mip0 (max resolution), 1 - mip1 (high resolution), etc...
     * \return The sampled bitmap pixel.
     */
    byte Sample(CHMFace::_enum faceType, float u, float v, int lod) const;

    /**
     * \brief Gets the LoD count.
     * \return The LoD count.
     */
    int GetLoDCount() const
    {
        return static_cast<int>(m_faces[0].MipMaps.Size());
    }

public:
    static RefPtr<CHMBitmap> CreateFromFile(const char* fileName);
};

#endif // CHMTEXTURE_H
