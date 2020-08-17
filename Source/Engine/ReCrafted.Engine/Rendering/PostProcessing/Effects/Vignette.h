// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Rendering/PostProcessing/PostProcessBase.h"

class Vignette final : public PostProcessBase
{
private:
    Shader* m_shader = nullptr;

protected:
    ~Vignette() override;

protected:
    void OnInitialize() override;

public:
    Shader* GetShader() const override;
};
