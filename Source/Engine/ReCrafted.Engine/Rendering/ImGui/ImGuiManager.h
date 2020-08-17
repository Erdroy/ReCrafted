// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Rendering/RenderingComponent.h"

class ImGuiManager : public RenderingComponent<ImGuiManager>
{
    friend class RenderingManager;

protected:
    void Initialize() override;
    void Shutdown() override;
    void Render() override;

    void BeginRender();
    void EndRender();

public:
    RenderingComponentStage GetStage() override
    {
        return RenderingComponentStage::Final;
    }
};
