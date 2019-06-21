// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/List.h"
#include "Common/Array.h"
#include "Scripting/Object.h"
#include "Rendering/Shader.h"

API_USING("ReCrafted.API.Mathematics")

/// <summary>
///     Material class.
/// </summary>
API_CLASS(public, sealed, partial)
class Material : public Object
{
    API_CLASS_BODY()

private:
    Shader* m_shader = nullptr;

    // TODO: Data, Textures etc.

public:

    /// <summary>
    ///     Gets the shader that this material uses.
    /// </summary>
    API_PROPERTY(noprefix)
    Shader* GetShader() const
    {
        return m_shader;
    }
};