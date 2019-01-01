// ReCrafted (c) 2016-2019 Always Too Late

#include "SpaceObjectTables.hpp"

byte LocalNeighTable[8] = {
    0b010101u,
    0b011001u,
    0b101001u,
    0b100101u,
    0b010110u,
    0b011010u,
    0b101010u,
    0b100110u
};

byte NeighDirTable[8][3] = {
    {3, 1, 4}, // 0
    {2, 0, 5}, // 1
    {1, 3, 6}, // 2
    {0, 2, 7}, // 3

    {7, 5, 0}, // 4
    {6, 4, 1}, // 5
    {5, 7, 2}, // 6
    {4, 6, 3}  // 7
};


byte NodeDirIds[8] = {
    7, // (---)
    4, // (--Z)
    3, // (-Y-)
    0, // (-YZ)
    6, // (X--)
    5, // (X-Z)
    2, // (XY-)
    1, // (XYZ)
};

Vector3 ChildrenNodeOffsets[8] = {
    Vector3(-0.5f, 0.5f, 0.5f), // 0 (-YZ)
    Vector3(0.5f, 0.5f, 0.5f),  // 1 (XYZ)
    Vector3(0.5f, 0.5f, -0.5f), // 2 (XY-)
    Vector3(-0.5f, 0.5f, -0.5f),// 3 (-Y-)

    Vector3(-0.5f, -0.5f, 0.5f),// 4 (--Z)
    Vector3(0.5f, -0.5f, 0.5f), // 5 (X-Z)
    Vector3(0.5f, -0.5f, -0.5f),// 6 (X--)
    Vector3(-0.5f, -0.5f, -0.5f)// 7 (---)
};

Vector3 DirectionOffset[6] = {
    Vector3(0.0f, 0.0f, 1.0f),
    Vector3(0.0f, 0.0f, -1.0f),
    Vector3(-1.0f, 0.0f, 0.0f),
    Vector3(1.0f, 0.0f, 0.0f),
    Vector3(0.0f, 1.0f, 0.0f),
    Vector3(0.0f, -1.0f, 0.0f)
};

int LoDTable[22] = {
    0,
    1,
    2,
    4,
    8,
    16,
    32,
    64,
    128,
    256,
    512,
    1024,
    2048,
    4096,
    8192,
    16384,
    32768,
    65536,
    131072,
    262144,
    524288,
    1048576
};

Color NodeLoDDebugColors[14] = {
    Color(255, 0, 0, 0), // 0
    Color(255, 32, 0, 0), // 1
    Color(230, 64, 0, 16), // 2
    Color(200, 128, 0, 32), // 3
    Color(128, 128, 0, 64), // 4
    Color(64, 128, 0, 64), // 5
    Color(32, 170, 0, 64), // 6
    Color(16, 200, 0, 128), // 7
    Color(0, 255, 0, 128), // 8
    Color(0, 255, 0, 128), // 9
    Color(0, 255, 0, 128), // 10
    Color(0, 255, 0, 128), // 11
    Color(0, 255, 0, 128), // 12
    Color(0, 255, 0, 128), // 13
};