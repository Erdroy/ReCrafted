// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef PPVIGNETTE_H
#define PPVIGNETTE_H

#include "PostProcessingBase.h"

class PPVignette : public PostProcessingBase
{
private:
    RefPtr<Shader> m_shader = nullptr;

protected:
    void OnInitialize() override;
    void Render() override;
    void Dispose() override; 
    
private:
    RefPtr<Shader> GetShader() const override;
};

#endif // PPVIGNETTE_H
