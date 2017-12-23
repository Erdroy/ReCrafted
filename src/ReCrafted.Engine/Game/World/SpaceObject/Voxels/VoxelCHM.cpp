// ReCrafted © 2016-2017 Always Too Late

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
        Logger::logError("VoxelCHM::loadFromDirectory() invalid bits per pixel count got! ({0} expected 8)", bitsPerPixel);
        return;
    }

    // set width and height of this bitmap TODO: check if all faces has the same size?
    m_bitmapWidth = bitmapWidth;
    m_bitmapHeight = bitmapHeight;

    // set faces
    m_faces[face] = bitmap;
}

Ptr<VoxelCHM> VoxelCHM::loadFromDirectory(const char* directoryName)
{
    Ptr<VoxelCHM> chm(new VoxelCHM);

    chm->loadFace(0, "/front.bmp", directoryName);
    chm->loadFace(1, "/back.bmp", directoryName);
    chm->loadFace(2, "/left.bmp", directoryName);
    chm->loadFace(3, "/right.bmp", directoryName);
    chm->loadFace(4, "/up.bmp", directoryName);
    chm->loadFace(5, "/down.bmp", directoryName);

    return chm;
}
