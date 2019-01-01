// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef POSTPROCESSINGBASE_H
#define POSTPROCESSINGBASE_H

// includes
#include "ReCrafted.h"
#include "Graphics/Shader.h"

class PostProcessingBase : IDisposable
{
    friend class Rendering;

private:
    virtual void OnInitialize() = 0;
    virtual void Render() = 0;

public:
    virtual Shader* GetShader() const = 0;
};

#endif // POSTPROCESSINGBASE_H
