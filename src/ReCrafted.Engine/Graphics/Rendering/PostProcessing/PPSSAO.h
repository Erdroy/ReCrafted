// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef PPSSAO_H
#define PPSSAO_H

#include "PostProcessingBase.h"

class PPSSAO : public PostProcessingBase
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

#endif // PPSSAO_H
