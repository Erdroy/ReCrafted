// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Rendering/Materials/MaterialFieldType.h"

/// <summary>
///     Basic Material field description. Contains all data needed to specify and upload a field to shader.
/// </summary>
class MaterialField
{
    DELETE_COPY_MOVE(MaterialField)

public:
    /// <summary>
    ///     The material field name.
    /// </summary>
    std::string Name;

    /// <summary>
    ///     The material field data.
    /// </summary>
    uint8_t* Data;

    /// <summary>
    ///     The material field data size (in bytes).
    /// </summary>
    uint8_t Size;

    /// <summary>
    ///     The material field type.
    /// </summary>
    MaterialFieldType Type;

public:
    /// <summary>
    ///     Default MaterialField constructor.
    /// </summary>
    MaterialField() : Data(nullptr), Size(0u), Type(MaterialFieldType::Unknown) {}

    /// <summary>
    ///     Default MaterialField destructor.
    /// </summary>
    ~MaterialField()
    {
        delete Data;
        Size = 0u;
        Type = MaterialFieldType::Unknown;
    }
};