// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef PPVIGNETTE_H
#define PPVIGNETTE_H

#include "PostProcessingBase.h"

class PPVignette : public PostProcessingBase
{
private:
    Shader* m_shader = nullptr;

protected:
    void OnInitialize() override;
    void Render() override;
    void Dispose() override; 
    
private:
    Shader* GetShader() const override;
};

#endif // PPVIGNETTE_H
