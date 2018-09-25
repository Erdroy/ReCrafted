// ReCrafted (c) 2016-2018 Always Too Late

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
    virtual RefPtr<Shader> GetShader() const = 0;
};

#endif // POSTPROCESSINGBASE_H
