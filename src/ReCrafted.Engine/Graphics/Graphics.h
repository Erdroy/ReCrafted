// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef GRAPHICS_H
#define GRAPHICS_H

// includes
#include "ReCrafted.h"
#include "Core/EngineComponent.h"
#include "Graphics/Shader.h"
#include "Graphics/RenderBuffer.h"
#include "Graphics/Mesh.h"
#include "Graphics/RenderStage.h"
#include "Renderer/Renderer.hpp"
#include "Rendering/Rendering.h"
#include "Rendering/PostProcessing/PPVignette.h"
#include "Rendering/PostProcessing/PPSSAO.h"

/**
 * \brief Graphics class.
 */
class Graphics : public EngineComponent<Graphics>
{
    friend class EngineMain;

private:
SCRIPTING_API_IMPL()

private:
    bool m_wireframe = false;

    RefPtr<Rendering> m_rendering = nullptr;
    RefPtr<Shader> m_currentShader = nullptr;

    Array<RefPtr<Texture2D>> m_currentTextures = {};

    RefPtr<RenderBuffer> m_gbuffer = nullptr;
    RefPtr<Shader> m_gbufferFillShader = nullptr;
    RefPtr<Shader> m_gbufferCombine = nullptr;

    RefPtr<PPVignette> m_vignette = nullptr;
    RefPtr<PPSSAO> m_ssao = nullptr;

    RenderStage::_enum m_renderStage = RenderStage::Default;

    Renderer::WindowHandle m_window = {};
    Renderer::Texture2DHandle m_frameTexture = {};
    Renderer::RenderBufferHandle m_frameBuffer = {};

public:
    virtual ~Graphics() = default;

private:
    void LoadInternalShaders();
    void CreateRenderBuffers();

    void InitializeRenderer();
    void InitializePostProcessing();

    void UpdateDefaultConstants(const Matrix& mvp);

    void BlitFrameBuffer();

private:
    void OnInit() override;
    void OnDispose() override;
    void Update() override;

    void OnFramePresent();

    void Render();
    void Resize(uint width, uint height);

private:
    void RenderBegin();
    void RenderEnd();

    void RenderDebugDraw();
    void RenderUI();

public:
    /**
     * \brief Draws given mesh using given shader.
     * \param mesh The mesh class pointer.
     * \param shader The shader which will be used to draw this mesh.
     */
    void Draw(const RefPtr<Mesh>& mesh, const RefPtr<Shader>& shader);

    /**
    * \brief Draws given mesh using current shader.
    * \param mesh The mesh class pointer.
    */
    void Draw(const RefPtr<Mesh>& mesh);

    /**
     * \brief Sets given shader as current.
     */
    void SetShader(const RefPtr<Shader>& shader);

    /**
    * \brief Sets given stage as current.
    */
    void SetStage(RenderStage::_enum stage);

    /**
     * \brief Sets given texture at given slot.
     * \param slot The shader slot.
     * \param texture2d The texture.
     */
    void SetTexture(uint slot, const RefPtr<Texture2D>& texture2d);

    /**
    * \brief Gets current set render stage.
    */
    RenderStage::_enum GetStage() const
    {
        return m_renderStage;
    }

public:
    /**
     * \brief Takes screenshot of the already rendered frame.
     * \param fileName The file name to output the screenshot.
     */
    static void Screenshot(Text fileName);
};

#endif // GRAPHICS_H
