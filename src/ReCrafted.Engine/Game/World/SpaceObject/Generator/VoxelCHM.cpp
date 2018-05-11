// ReCrafted (c) 2016-2018 Always Too Late

#include "VoxelCHM.h"
#include "Graphics/Bitmap.h"

void VoxelCHM::loadFace(const int face, const char* fileName, const char* directoryName)
{
    char buffer[512];
    memset(buffer, 0, 512);
    strcat(buffer, directoryName);
    strcat(buffer, fileName);

    int bitmapWidth;
    int bitmapHeight;
    int bitsPerPixel;
    cvar bitmap = Bitmap::load(buffer, &bitmapWidth, &bitmapHeight, &bitsPerPixel);

    if (bitsPerPixel != 8)
    {
        Logger::logError("VoxelCHM::loadFromDirectory() invalid bits per pixel count got! ({0} expected 8)",
                         bitsPerPixel);
        return;
    }

    // TODO: bitmap downsampling

    // set width and height of this bitmap TODO: check if all faces has the same size?
    m_bitmapWidth = bitmapWidth;
    m_bitmapHeight = bitmapHeight;

    // set faces
    m_faces[face] = bitmap;
}

Ref<VoxelCHM> VoxelCHM::loadFromDirectory(const char* directoryName)
{
    Ref<VoxelCHM> chm(new VoxelCHM);

    chm->loadFace(0, "/left.bmp", directoryName);
    chm->loadFace(1, "/right.bmp", directoryName);
    chm->loadFace(2, "/up.bmp", directoryName);
    chm->loadFace(3, "/down.bmp", directoryName);
    chm->loadFace(4, "/front.bmp", directoryName);
    chm->loadFace(5, "/back.bmp", directoryName);

    return chm;
}
