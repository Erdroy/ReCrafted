// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef VOXELSTORAGEHEADERENTRY_H
#define VOXELSTORAGEHEADERENTRY_H

// includes
#include "ReCrafted.h"

/**
* \brief VoxelStorageChunkEntry structure.
*/
struct VoxelStorageChunkEntry
{
    uint8_t storageId;
    uint32_t storageOffset;
};

#endif // VOXELSTORAGEHEADERENTRY_H