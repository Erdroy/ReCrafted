// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/List.h"
#include "Core/SubSystems/SubSystem.h"
#include "Rendering/Renderer/Renderer.h"

class PostProcessingManager : public SubSystem<PostProcessingManager>
{
    DELETE_COPY_MOVE(PostProcessingManager)

private:
    List<PostProcessBase*> m_effects = {};

    Renderer::Texture2DHandle m_buffer0 = {};
    Renderer::Texture2DHandle m_buffer1 = {};

public:
    PostProcessingManager() = default;

private:
    void OnResize(uint width, uint height);
    void CreateBuffers(uint width, uint height);
    void DestroyBuffers();

protected:
    void Initialize() override;
    void Shutdown() override;

public:
    void RenderAllEffects(
        const Renderer::Texture2DHandle& colorTexture, 
        const Renderer::Texture2DHandle& normalsTexture, 
        const Renderer::Texture2DHandle& depthTexture);

    void AddPostProcess(PostProcessBase* postProcess);

public:
    template<class TPostProcess>
    static const TPostProcess* AddPostProcess()
    {
        static_assert(std::is_base_of<PostProcessBase, TPostProcess>::value, "TPostProcess must inherit from PostProcessBase");
        const auto postProcess = new TPostProcess();
        GetInstance()->AddPostProcess(postProcess);
        return postProcess;
    }

    template<class TPostProcess>
    static void RemovePostProcess(TPostProcess* effect)
    {
        static_assert(std::is_base_of<PostProcessBase, TPostProcess>::value, "TPostProcess must inherit from PostProcessBase");

    }

};
