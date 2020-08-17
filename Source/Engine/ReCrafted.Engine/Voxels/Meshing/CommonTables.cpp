// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "CommonTables.hpp"

Vector3i CellCorner[8] = {
    Vector3i(0, 0, 0),
    Vector3i(1, 0, 0),
    Vector3i(0, 0, 1),
    Vector3i(1, 0, 1),
    Vector3i(0, 1, 0),
    Vector3i(1, 1, 0),
    Vector3i(0, 1, 1),
    Vector3i(1, 1, 1)
};

Vector4 EncodedMaterials[16] = {
    Vector4(1.0f, 0.0f, 0.0f, 0.0f),
    Vector4(0.0f, 1.0f, 0.0f, 0.0f),
    Vector4(0.0f, 0.0f, 1.0f, 0.0f),
    Vector4(0.0f, 0.0f, 0.0f, 1.0f),

    Vector4(1.0f, 0.0f, 0.0f, 0.0f),
    Vector4(0.0f, 1.0f, 0.0f, 0.0f),
    Vector4(0.0f, 0.0f, 1.0f, 0.0f),
    Vector4(0.0f, 0.0f, 0.0f, 1.0f),

    Vector4(1.0f, 0.0f, 0.0f, 0.0f),
    Vector4(0.0f, 1.0f, 0.0f, 0.0f),
    Vector4(0.0f, 0.0f, 1.0f, 0.0f),
    Vector4(0.0f, 0.0f, 0.0f, 1.0f),

    Vector4(1.0f, 0.0f, 0.0f, 0.0f),
    Vector4(0.0f, 1.0f, 0.0f, 0.0f),
    Vector4(0.0f, 0.0f, 1.0f, 0.0f),
    Vector4(0.0f, 0.0f, 0.0f, 1.0f)
};
