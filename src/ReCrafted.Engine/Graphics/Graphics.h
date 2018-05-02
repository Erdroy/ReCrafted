// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef GRAPHICS_H
#define GRAPHICS_H

// includes
#include "ReCrafted.h"
#include "Core/EngineComponent.h"
#include "Graphics/Shader.h"
#include "Graphics/Mesh.h"
#include "RenderStage.h"

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
    int m_viewId = 0;
    bool m_wireframe = false;
    Ptr<Shader> m_currentShader = nullptr;
    RenderStage::_enum m_renderStage = RenderStage::Default;

    Renderer::WindowHandle m_window = {};
    Renderer::RenderBufferHandle m_frameBuffer = {};

public:
    virtual ~Graphics() = default;

private:
    void loadInternalShaders();
    void createUniforms();
    void createRenderBuffers();

    void initializeRenderer();

private:
    void onInit() override;
    void onDispose() override;
    void update() override;

    void render();
    void resize(uint width, uint height);

private:
    void renderBegin();
    void renderEnd();
    void renderWorld();
    void renderUI();

public:
    /**
     * \brief Draws given mesh using current shader, view and matrix.
     * \param mesh The mesh class pointer.
     */
    void draw(Ptr<Mesh>& mesh);

    /**
     * \brief Sets given shader as current.
     */
    void setShader(Ptr<Shader>& shader);

    /**
    * \brief Sets given view id as current.
    */
    void setView(int viewId);

    /**
    * \brief Sets given matrix as current.
    */
    void setMatrix(Matrix& mvpMatrix);

    /**
    * \brief Sets given stage as current.
    */
    void setStage(RenderStage::_enum stage);

    /**
    * \brief Gets current set render stage.
    */
    RenderStage::_enum getStage() const
    {
        return m_renderStage;
    }
};

#endif // GRAPHICS_H
