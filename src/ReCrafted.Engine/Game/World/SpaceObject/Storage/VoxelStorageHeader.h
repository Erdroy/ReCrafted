// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef VOXELSTORAGEHEADER_H
#define VOXELSTORAGEHEADER_H

// includes
#include "ReCrafted.h"

/**
* \brief VoxelStorageHeader structure.
*/
struct VoxelStorageHeader
{
public:
    /**
     * \brief The signature of the header.
     */
    char signature[7] = { 'V', 'X', 'H', ' ', '1', '.', '0' };
    
    /**
     * \brief Amount of chunks that can be saved in single voxel storage *.vx file.
     */
    uint16_t voxelFileCapacity = 4096u; // 4096 chunks per file, which gives (4096 * (16*16*16)) - 16 MiB

    /**
     * \brief Amount of voxel storage *.vx files.
     */
    uint16_t voxelFileCount = 0u;

    /**
     * \brief The chunk map size (amount of chunk entries in chunk map)
     */
    uint32_t chunkMapSize = 0u;
};

#endif // VOXELSTORAGEHEADER_H
