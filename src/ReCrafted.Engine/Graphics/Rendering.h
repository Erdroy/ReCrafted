// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef RENDERING_H
#define RENDERING_H

// includes
#include "ReCrafted.h"
#include "Core/EngineComponent.h"

#include "bgfxPrerequisites.h"

#include "Core/Defines.h"
#include "Core/Types.h"
#include "Mesh.h"
#include "Shader.h"
#include "RenderBuffer.h"

/**
 * \brief Rendering class.
 */
class Rendering : public EngineComponent<Rendering>
{
    friend class EngineMain;

private:
    bool m_wireframe = false;

	bgfx::UniformHandle m_modelViewProjection = {};
	bgfx::UniformHandle m_texture0 = {};
	bgfx::UniformHandle m_texture1 = {};
	bgfx::UniformHandle m_texture2 = {};
	bgfx::UniformHandle m_texture3 = {};

	Ptr<RenderBuffer> m_gbuffer = nullptr;
	Ptr<Mesh> m_blitMesh = nullptr;
 	Ptr<Shader> m_blitShader = nullptr;
	Ptr<Shader> m_gbufferShader = nullptr;
	Ptr<Shader> m_deferredFinal = nullptr;

private:
	void loadInternalShaders();
	void createUniforms();
	void createRenderBuffers();
	void createBlitQuad();

private:
    void onInit() override;
    void onDispose() override;

    void update();

    void beginRender();
    void endRender();

    void renderWorld();
    void renderUI();

public:
    virtual ~Rendering() = default;

public:
    void render();
    
    /// <summary>
	/// Resize
	/// </summary>
	/// <param name="width">Width</param>
	/// <param name="height">Height</param>
	void resize(uint width, uint height);

	/// <summary>
	/// Draw mesh using custom shader.
	/// </summary>
	/// <param name="mesh">Mesh to draw.</param>
	/// <param name="shader">Shader that will be used to shade the mesh.</param>
	/// <param name="modelMatrix">Matrix which includes translation, rotation and scale of this mesh for rendering.</param>
	void draw(Ptr<Mesh>& mesh, Ptr<Shader>& shader, Matrix* modelMatrix = nullptr, int view = RENDERVIEW_BACKBUFFER);

	/// <summary>
	/// Draw mesh using gbuffer shader.
	/// </summary>
	/// <param name="mesh">Mesh to draw.</param>
	/// <param name="modelMatrix">Matrix which includes translation, rotation and scale of this mesh for rendering.</param>
	void draw(Ptr<Mesh>& mesh, Matrix* modelMatrix = nullptr);

	//void draw(Ptr<Entity> entity);

	/// <summary>
	/// Draw texture on fullscreen quad.
	/// </summary>
	/// <param name="view">The target view.</param>
	/// <param name="texture">The texture you want to render.</param>
	void blit(uint view, bgfx::TextureHandle texture);

	void setWorldMatrice(Matrix& modelMatrix);

	/// <summary>
	/// Set rendering state.
	/// </summary>
	/// <param name="tristrip">Enable triangle strip primitives?</param>
	/// <param name="msaa">Allow MSAA?</param>
	void setState(bool tristrip = false, bool msaa = true, bool uiRendering = false, bool debugLines = false, bool debugTriangles = false);
};

#endif // RENDERING_H