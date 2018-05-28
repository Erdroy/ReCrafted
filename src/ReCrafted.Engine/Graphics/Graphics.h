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

#include "Graphics/Renderer/Renderer.hpp"

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

    Ref<Shader> m_currentShader = nullptr;

    Ref<RenderBuffer> m_gbuffer = nullptr;
    Ref<Shader> m_gbufferFillShader = nullptr;
    Ref<Shader> m_gbufferCombine = nullptr;

    RenderStage::_enum m_renderStage = RenderStage::Default;

    Renderer::WindowHandle m_window = {};
    Renderer::RenderBufferHandle m_frameBuffer = {};

public:
    virtual ~Graphics() = default;

private:
    void LoadInternalShaders();
    void CreateRenderBuffers();

    void InitializeRenderer();

private:
    void OnInit() override;
    void OnDispose() override;
    void Update() override;

    void Render();
    void Resize(uint width, uint height);

private:
    void RenderBegin();
    void RenderEnd();
    void RenderWorld();
    void RenderUI();

public:
    /**
     * \brief Draws given mesh using current shader, view and matrix.
     * \param mesh The mesh class pointer.
     */
    void Draw(Ref<Mesh>& mesh);

    /**
     * \brief Sets given shader as current.
     */
    void SetShader(Ref<Shader>& shader);

    /**
    * \brief Sets given stage as current.
    */
    void SetStage(RenderStage::_enum stage);

    /**
    * \brief Gets current set render stage.
    */
    RenderStage::_enum GetStage() const
    {
        return m_renderStage;
    }
};

#endif // GRAPHICS_H
