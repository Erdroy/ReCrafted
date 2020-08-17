// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "VoxelLookupTables.h"

byte VoxelLookup::LocalNeighTable[8] = {
    0b010101u,
    0b011001u,
    0b101001u,
    0b100101u,
    0b010110u,
    0b011010u,
    0b101010u,
    0b100110u
};

byte VoxelLookup::NeighDirTable[8][3] = {
    {3, 1, 4}, // 0
    {2, 0, 5}, // 1
    {1, 3, 6}, // 2
    {0, 2, 7}, // 3

    {7, 5, 0}, // 4
    {6, 4, 1}, // 5
    {5, 7, 2}, // 6
    {4, 6, 3}  // 7
};


byte VoxelLookup::NodeDirIds[8] = {
    7, // (---)
    4, // (--Z)
    3, // (-Y-)
    0, // (-YZ)
    6, // (X--)
    5, // (X-Z)
    2, // (XY-)
    1, // (XYZ)
};

Vector3d VoxelLookup::ChildrenNodeOffsets[8] = {
    Vector3d(-0.5, 0.5, 0.5), // 0 (-YZ)
    Vector3d(0.5, 0.5, 0.5),  // 1 (XYZ)
    Vector3d(0.5, 0.5, -0.5), // 2 (XY-)
    Vector3d(-0.5, 0.5, -0.5),// 3 (-Y-)

    Vector3d(-0.5, -0.5, 0.5),// 4 (--Z)
    Vector3d(0.5, -0.5, 0.5), // 5 (X-Z)
    Vector3d(0.5, -0.5, -0.5),// 6 (X--)
    Vector3d(-0.5, -0.5, -0.5)// 7 (---)
};

Vector3d VoxelLookup::DirectionOffset[6] = {
    Vector3d(0.0, 0.0, 1.0),
    Vector3d(0.0, 0.0, -1.0),
    Vector3d(-1.0, 0.0, 0.0),
    Vector3d(1.0, 0.0, 0.0),
    Vector3d(0.0, 1.0, 0.0),
    Vector3d(0.0, -1.0, 0.0)
};
