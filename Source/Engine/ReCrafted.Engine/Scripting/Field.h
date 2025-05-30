// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Scripting/Mono.h"


/// <summary>
///     The FieldType enum.
/// </summary>
enum class FieldType
{
    Void = 0x01,
    Bool = 0x02,
    Char = 0x03,
    I1 = 0x04,
    U1 = 0x05,
    I2 = 0x06,
    U2 = 0x07,
    I4 = 0x08,
    U4 = 0x09,
    I8 = 0x0a,
    U8 = 0x0b,
    R4 = 0x0c,
    R8 = 0x0d,
    String = 0x0e,
    Ptr = 0x0f,
    ByRef = 0x10,
    ValueType = 0x11,
    Class = 0x12,
    Var = 0x13,
    Array = 0x14,
    Enum = 0x55,
};

/// <summary>
///     Field structure. Represents Mono scripting backend field.
/// </summary>
struct Field
{
private:
    MonoClassField* m_field = nullptr;

public:
    /// <summary>
    ///     Initializes Field structure instance.
    /// </summary>
    /// <param name="field">The mono field reference.</param>
    explicit Field(MonoClassField* field) : m_field(field)
    {
    }

public:
    /// <summary>
    ///     Sets value.
    /// </summary>
    void SetValue(Object* instance, void* value) const;

    /// <summary>
    ///     Gets value.
    /// </summary>
    void GetValue(Object* instance, void* value) const;

    /// <summary>
    ///     Gets field type.
    /// </summary>
    FieldType GetType() const;

    /// <summary>
    ///     Returns true when valid.
    /// </summary>
    bool IsValid() const;

    /// <summary>
    ///     Returns Mono field pointer.
    /// </summary>
    /// <returns>The mono's field pointer.</returns>
    MonoClassField* ToMono() const;
};