// ReCrafted (c) 2016-2018 Always Too Late

#include "CHMBitmap.h"

#include "Graphics/IResource.h"
#include "Core/Streams/Stream.h"
#include "Core/Streams/FileStream.h"
#include "Core/Streams/BinaryStream.h"

struct ALIGN(8) CHMTextureHeader
{
    char Signature[8]; // RCCHM10

    uint16_t Width;
    uint16_t Height;
    uint16_t Reserved0;
    uint8_t MipNum;
    uint8_t Reserved1;
};
STATIC_ASSERT(sizeof(CHMTextureHeader) == 16);

byte CHMBitmap::FetchBitmapTexelScalar(const FaceBitmap& mip, const int& x, const int& y) const
{
    cvar offset = x + y * mip.Height;
    return mip.Bitmap[offset];
}

CHMBitmap::CHMBitmap()
{
    m_faces = {};

    for (var i = 0; i < 6; i++)
        m_faces.Add(Face());
}

void CHMBitmap::Dispose()
{
    for (var i = 0; i < 6; i++)
        m_faces[i].Dispose();

    m_faces.Clear();
}

byte CHMBitmap::Sample(CHMFace::_enum faceType, float u, float v, int lod) const
{
    DEBUG_ASSERT(u >= 0.0f && u < 1.0f); // Well... make sure that Erdroy can read his comments.
    DEBUG_ASSERT(v >= 0.0f && v < 1.0f);

    // NOTE: This function MUST be fast as fuck, really.
    // TODO: Inline (CHMTexture.inl)
    // TODO: SSE Sampling

    // Sample using Scalar Bilinear Interplation
    crvar face = m_faces[faceType];
    crvar mip = face.MipMaps[lod]; // We can directly use lod, as we've mentioned about the lod restrictions in function's comment, yey.

    cvar px = uint16_t(u * mip.Width);
    cvar py = uint16_t(v * mip.Height);

    cvar x = float(u * mip.Width);
    cvar y = float(v * mip.Height);

    cvar needFlipX = px + 1 >= mip.Width; // x + 1 == Width: sample mip going right
    cvar needFlipY = py + 1 >= mip.Height; // y + 1 == Height-1: sample mip going down (warning: we need to properly swap axis!)

    byte texel0 = 0u;
    byte texel1 = 0u;
    byte texel2 = 0u;
    byte texel3 = 0u;

    // Fetch texels
    // Check if we need to flip, if not, just fetch texel from current mip
    if(!needFlipX && !needFlipY)
    {
        // Sample simple
        texel0 = FetchBitmapTexelScalar(mip, px, py);
        texel1 = FetchBitmapTexelScalar(mip, px + 1, py);
        texel2 = FetchBitmapTexelScalar(mip, px, py + 1);
        texel3 = FetchBitmapTexelScalar(mip, px + 1, py + 1);
    }
    else if (needFlipX && !needFlipY) // If needs second texel flip, fetch neighbouring faces
    {
        // TODO: Flip and Sample 1 neigh (X)
    }
    else if (!needFlipX && needFlipY)
    {
        // TODO: Flip and Sample 1 neigh (Y)
    }
    else
    {
        // TODO: Flip and Sample 3 neighs (X, Y, XY)
    }

    cvar fx = x - px;
    cvar fy = y - py;
    cvar fx1 = 1.0f - fx;
    cvar fy1 = 1.0f - fy;

    // Calculate weights
    cvar w1 = float(fx1 * fy1 * 256.0f);
    cvar w2 = float(fx * fy1 * 256.0f);
    cvar w3 = float(fx1 * fy  * 256.0f);
    cvar w4 = float(fx * fy  * 256.0f);
    
    cvar out = int(texel0 * w1 + texel1 * w2 + texel2 * w3 + texel3 * w4);

    return byte(out >> 8);
}

RefPtr<CHMBitmap> CHMBitmap::CreateFromFile(const char* fileName)
{
    _ASSERT_(Platform::FileExists(fileName), "Specified CHM texture is missing!");

    // Create bitmap
    RefPtr<CHMBitmap> bitmap(new CHMBitmap());

    var fs = FileStream(fileName, OpenMode::OpenRead);
    var stream = BinaryStream(fs);

    cvar header = stream.ReadStruct<CHMTextureHeader>();
    ASSERT(strcmp(header.Signature, "RCCHM10") == 0); // check signature

    // Read all face bitmaps and mips
    var mipSize = header.Width;
    for(var i = 0u; i < header.MipNum; i ++)
    {
        for(var j = 0u; j < 6; j ++)
        {
            ASSERT(mipSize > 0);

            rvar face = bitmap->m_faces[j];

            // Allocate face data
            cvar faceDataSize = size_t(mipSize * mipSize);
            cvar faceData = new byte[faceDataSize];

            // Read face data
            stream.ReadBytes(reinterpret_cast<char*>(faceData), faceDataSize);

            FaceBitmap faceBitmap;
            faceBitmap.Width = mipSize;
            faceBitmap.Height = mipSize;
            faceBitmap.Bitmap = faceData;

            face.MipMaps.Add(faceBitmap);
        }

        // Set next mip map size
        mipSize /= 2;
    }

    // Dispose streams
    stream.Dispose();
    fs.Dispose();

    return bitmap;
}
