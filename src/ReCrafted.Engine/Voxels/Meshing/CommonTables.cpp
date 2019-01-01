// ReCrafted (c) 2016-2019 Always Too Late

#ifndef COMMONTABLES_CPP
#define COMMONTABLES_CPP

#include "CommonTables.hpp"

Int3 CellCorner[8] = {
    Int3(0, 0, 0),
    Int3(1, 0, 0),
    Int3(0, 0, 1),
    Int3(1, 0, 1),
    Int3(0, 1, 0),
    Int3(1, 1, 0),
    Int3(0, 1, 1),
    Int3(1, 1, 1)
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

#endif // COMMONTABLES_CPP
