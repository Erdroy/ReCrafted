// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

class VoxelLookup
{
public:
    static byte LocalNeighTable[];
    static byte NeighDirTable[][3];
    static byte NodeDirIds[];
    static Vector3d DirectionOffset[];
    static Vector3d ChildrenNodeOffsets[];
};