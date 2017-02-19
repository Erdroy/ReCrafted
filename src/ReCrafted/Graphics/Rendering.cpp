// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Rendering.h"
#include "Camera.h"
#include "../Core/GameCore.h"

Rendering* Rendering::m_instance;

bgfx::UniformHandle _wvp;

void Rendering::loadInternalShaders()
{
}

void Rendering::createUniforms()
{
	// create uniforms
	_wvp = bgfx::createUniform("m_modelViewProjection", bgfx::UniformType::Mat4);
}

void Rendering::createRenderBuffers()
{
	// create render buffer for geometry pass
	m_gbuffer = RenderBuffer::createRenderTarget();
	m_gbuffer->begin();
	m_gbuffer->addTarget("ALBEDO", TextureFormat::RGBA8);
	m_gbuffer->addTarget("DEPTh", TextureFormat::D24);
	m_gbuffer->end();
}

void Rendering::createBlitQuad()
{
	// full screen quad vertex buffer decl
	bgfx::VertexDecl decl = {};
	decl.begin();
	decl.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);
	decl.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float);
	decl.end();

	// the structure for the quad
	struct fsqVdata
	{
		Vector3 position;
		Vector2 uv;
	};

	// quad vertex data
	static fsqVdata data[4] = {
		{ Vector3(1.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f) },
		{ Vector3(-1.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f) },
		{ Vector3(1.0f,-1.0f, 0.0f), Vector2(0.0f, 0.0f) },
		{ Vector3(-1.0f,-1.0f, 0.0f), Vector2(0.0f, 0.0f) }
	};

	// create vertex buffer for fullscreen quad
	auto memory = bgfx::alloc(decl.getStride() * 4);

	// copy data
	memcpy(memory->data, data, decl.getStride() * 4);

	m_quadvb = bgfx::createVertexBuffer(memory, decl);
}

void Rendering::init()
{
	// create uniforms
	createUniforms();

	// create render buffers
	createRenderBuffers();

	// create quad
	createBlitQuad();

	// load all shaders
	loadInternalShaders();
}

void Rendering::resize(uint width, uint height)
{
	if (Camera::m_mainCamera == nullptr)
	{
		VS_LOG("WARNING: Main camera is not set to.");
		return;
	}

	// update main camera perspective
	Camera::m_mainCamera->updatePerspective();
	
	m_gbuffer->resize(width, height);
}

void Rendering::beginRender()
{
	if(Camera::m_mainCamera == nullptr)
	{
		VS_LOG("WARNING: Trying to render scene without any camera set as main!");
		return;
	}

	// update main camera
	Camera::m_mainCamera->update();

}

void Rendering::endRender()
{
	// final pass
}

void Rendering::renderShadows()
{
}

void Rendering::renderStatic()
{
	m_gbuffer->bind();
}

void Rendering::renderEntities()
{
	m_gbuffer->bind();
}

void Rendering::draw(Ptr<Mesh> mesh, Ptr<Shader> shader, Matrix* modelMatrix)
{
	auto view = Camera::m_mainCamera->m_view;
	auto proj = Camera::m_mainCamera->m_projection;

	auto mat = *modelMatrix * view * proj;
	mat.transpose();

	bgfx::setUniform(_wvp, &mat);

	bgfx::setVertexBuffer(mesh->m_vertexBuffer);
	bgfx::setIndexBuffer(mesh->m_indexBuffer);

	bgfx::submit(RENDERVIEW_BACKBUFFER, shader->m_program);
}

void Rendering::draw(Ptr<Mesh> mesh, Matrix* modelMatrix)
{

}

void Rendering::blit(uint view, bgfx::TextureHandle texture)
{

}

void Rendering::dispose()
{
	m_gbuffer->dispose();

	bgfx::destroyUniform(_wvp);

	// suicide
	delete this;
}
