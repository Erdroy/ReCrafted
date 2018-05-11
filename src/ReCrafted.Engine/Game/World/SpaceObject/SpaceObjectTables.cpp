// ReCrafted (c) 2016-2018 Always Too Late

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
    {4, 6, 3} // 7
};

byte DirIndex[6] = {
    0, 0, 1, 1, 2, 2
};

byte NodeDirIds[8] = {
    7, // 0
    4, // 1
    3, // 2
    0, // 3
    6, // 4
    5, // 5
    2, // 6
    1, // 7
};

Vector3 DirectionOffset[6] = {
    Vector3(0.0f, 0.0f, 1.0f),
    Vector3(0.0f, 0.0f, -1.0f),
    Vector3(-1.0f, 0.0f, 0.0f),
    Vector3(1.0f, 0.0f, 0.0f),
    Vector3(0.0f, 1.0f, 0.0f),
    Vector3(0.0f, -1.0f, 0.0f)
};

Vector3 ChildrenNodeOffsets[8] = {
    Vector3(-0.5f, 0.5f, 0.5f),
    Vector3(0.5f, 0.5f, 0.5f),
    Vector3(0.5f, 0.5f, -0.5f),
    Vector3(-0.5f, 0.5f, -0.5f),

    Vector3(-0.5f, -0.5f, 0.5f),
    Vector3(0.5f, -0.5f, 0.5f),
    Vector3(0.5f, -0.5f, -0.5f),
    Vector3(-0.5f, -0.5f, -0.5f)
};

float lodtable[22] = {
    0.0f,
    1.0f,
    2.0f,
    4.0f,
    8.0f,
    16.0f,
    32.0f,
    64.0f,
    128.0f,
    256.0f,
    512.0f,
    1024.0f,
    2048.0f,
    4096.0f,
    8192.0f,
    16384.0f,
    32768.0f,
    65536.0f,
    131072.0f,
    262144.0f,
    524288.0f,
    1048576.0f
};
