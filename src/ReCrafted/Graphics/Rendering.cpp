// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Rendering.h"
#include "Camera.h"
#include "../Core/Logger.h"
#include "../Core/Input.h"
#include "Resources.h"
#include "../Game/Items/ItemDB.h"

Rendering* Rendering::m_instance;

void Rendering::loadInternalShaders()
{
	Logger::write("Loading internal shaders", LogLevel::Info);

	m_blitShader = Shader::loadShader("blit");
	m_gbufferShader = Shader::loadShader("gbuffer_standard");
	m_deferredFinal = Shader::loadShader("deferred_final");
}

void Rendering::createUniforms()
{
	Logger::write("Creating default uniforms", LogLevel::Info);

	// create uniforms
	m_modelViewProjection = bgfx::createUniform("m_modelViewProjection", bgfx::UniformType::Mat4);
	m_texture0 = bgfx::createUniform("m_texture0", bgfx::UniformType::Int1);
	m_texture1 = bgfx::createUniform("m_texture1", bgfx::UniformType::Int1);
	m_texture2 = bgfx::createUniform("m_texture2", bgfx::UniformType::Int1);
	m_texture3 = bgfx::createUniform("m_texture3", bgfx::UniformType::Int1);
}

void Rendering::createRenderBuffers()
{
	Logger::write("Creating render buffers", LogLevel::Info);

	// create render buffer for geometry pass
	m_gbuffer = RenderBuffer::createRenderTarget();
	m_gbuffer->begin();
	m_gbuffer->addTarget("ALBEDO", TextureFormat::RGBA8);
	m_gbuffer->addTarget("[RGB]NORMALS, [A]AmbientOcculusion", TextureFormat::RGBA8);
	m_gbuffer->addTarget("DEPTH", TextureFormat::D24);
	m_gbuffer->end();
}

void Rendering::createBlitQuad()
{
	static Vector3 vertices[4] = {
		Vector3(-1.0f, 1.0f, 0.0f),
		Vector3( 1.0f, 1.0f, 0.0f),
		Vector3( 1.0f,-1.0f, 0.0f),
		Vector3(-1.0f,-1.0f, 0.0f)
	};
	static Vector2 uvs[4] = {
		Vector2(0.0f,-1.0f),
		Vector2(1.0f,-1.0f),
		Vector2(1.0f, 0.0f),
		Vector2(0.0f, 0.0f)
	};

	static uint indices[6] = {
		2, 1, 0,
		3, 0, 2
	};

	m_blitMesh = Mesh::createMesh();
	m_blitMesh->setVertices(vertices, 4);
	m_blitMesh->setUVs(uvs);
	m_blitMesh->setIndices(indices, 6);
	m_blitMesh->applyChanges();
	m_blitMesh->upload();
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
	_ASSERT(Camera::m_mainCamera != nullptr);

	// update main camera perspective
	Camera::m_mainCamera->updatePerspective();
	
	m_gbuffer->resize(width, height);
}

void Rendering::beginRender()
{
	_ASSERT(Camera::m_mainCamera != nullptr);

	// update main camera
	Camera::m_mainCamera->update();

	// update shaders uniforms
	auto lightdir = Vector3(0.1f, -0.5f, 0.3f);
	m_deferredFinal->setValue(0, &lightdir);

}

void Rendering::endRender()
{
	setState(false, false);

	if(Input::isKey(Key_F1))
	{
		blit(0, m_gbuffer->getTarget(0));
		return;
	}

	if (Input::isKey(Key_F2))
	{
		blit(0, m_gbuffer->getTarget(1));
		return;
	}

	if (Input::isKey(Key_F3))
	{
		blit(0, m_gbuffer->getTarget(2));
		return;
	}

	// final pass
	auto textureFlags = 0 | BGFX_TEXTURE_MIN_POINT | BGFX_TEXTURE_MAG_POINT | BGFX_TEXTURE_MIP_POINT;

	bgfx::setTexture(0, m_texture0, m_gbuffer->getTarget(0), textureFlags);
	bgfx::setTexture(1, m_texture1, m_gbuffer->getTarget(1), textureFlags);

	// draw into backbuffer
	bgfx::setVertexBuffer(0, m_blitMesh->m_vertexBuffer);
	bgfx::setIndexBuffer(m_blitMesh->m_indexBuffer);
	bgfx::submit(RENDERVIEW_BACKBUFFER, m_deferredFinal->m_program);
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

void Rendering::draw(Ptr<Mesh> mesh, Ptr<Shader> shader, Matrix* modelMatrix, int viewId)
{
	auto view = Camera::m_mainCamera->m_view;
	auto proj = Camera::m_mainCamera->m_projection;

	auto mat = *modelMatrix * view * proj;
	mat.transpose();

	bgfx::setUniform(m_modelViewProjection, &mat);

	bgfx::setVertexBuffer(0, mesh->m_vertexBuffer);
	bgfx::setIndexBuffer(mesh->m_indexBuffer);
	bgfx::submit(viewId, shader->m_program);
}

void Rendering::draw(Ptr<Mesh> mesh, Matrix* modelMatrix)
{
	m_gbufferShader->setTexture(0, ItemDB::m_instance->m_atlas);
	draw(mesh, m_gbufferShader, modelMatrix, RENDERVIEW_GBUFFER);
}

void Rendering::blit(uint view, bgfx::TextureHandle texture)
{
	setState(false, false);

	auto textureFlags = 0 | BGFX_TEXTURE_MIN_POINT | BGFX_TEXTURE_MAG_POINT | BGFX_TEXTURE_MIP_POINT;

	bgfx::setTexture(0, m_texture0, texture, textureFlags);

	bgfx::setVertexBuffer(0, m_blitMesh->m_vertexBuffer);
	bgfx::setIndexBuffer(m_blitMesh->m_indexBuffer);
	bgfx::submit(view, m_blitShader->m_program);
}

void Rendering::setState(bool tristrip, bool msaa)
{
	auto state = 0;

	if (tristrip) 
		state |= BGFX_STATE_DEFAULT | BGFX_STATE_PT_TRISTRIP;
	else
		state |= BGFX_STATE_DEFAULT;

	if (msaa)
		state |= BGFX_STATE_MSAA;

	bgfx::setState(state);
}

void Rendering::dispose()
{
	Logger::write("Unloading rendering pipeline", LogLevel::Info);

	m_gbuffer->dispose();
	Logger::write("Unloaded render buffers", LogLevel::Info);
	
	m_blitMesh->dispose();

	m_blitShader->dispose();
	m_gbufferShader->dispose();
	m_deferredFinal->dispose();
	Logger::write("Unloaded shaders", LogLevel::Info);

	bgfx::destroyUniform(m_modelViewProjection);
	bgfx::destroyUniform(m_texture0);
	bgfx::destroyUniform(m_texture1);
	bgfx::destroyUniform(m_texture2);
	bgfx::destroyUniform(m_texture3);
	Logger::write("Unloaded uniforms", LogLevel::Info);

	// suicide
	delete this;
}
