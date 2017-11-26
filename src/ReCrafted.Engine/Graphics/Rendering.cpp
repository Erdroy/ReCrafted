// ReCrafted © 2016-2017 Always Too Late

#include "Rendering.h"
#include "Camera.h"
#include "Core/Logger.h"
#include "DebugDraw.h"
#include "Core/GameMain.h"

Rendering* Rendering::m_instance;

void Rendering::loadInternalShaders()
{
	Logger::logInfo("Loading internal shaders");

	m_blitShader = Shader::loadShader("blit");
	m_gbufferShader = Shader::loadShader("gbuffer_standard");
	m_deferredFinal = Shader::loadShader("deferred_final");
}

void Rendering::createUniforms()
{
	Logger::logInfo("Creating default uniforms");

	// create uniforms
	m_modelViewProjection = bgfx::createUniform("m_modelViewProjection", bgfx::UniformType::Mat4);
	m_texture0 = bgfx::createUniform("m_texture0", bgfx::UniformType::Int1);
	m_texture1 = bgfx::createUniform("m_texture1", bgfx::UniformType::Int1);
	m_texture2 = bgfx::createUniform("m_texture2", bgfx::UniformType::Int1);
	m_texture3 = bgfx::createUniform("m_texture3", bgfx::UniformType::Int1);
}

void Rendering::createRenderBuffers()
{
	Logger::logInfo("Creating render buffers");

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
		2, 0, 3
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
	lightdir.normalize();
	m_deferredFinal->setValue(0, &lightdir);

}

void Rendering::endRender()
{
	setState(false, false);
	
	if (Input::isKey(Key_F2))
	{
		blit(0, m_gbuffer->getTarget(1));
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

void Rendering::draw(Ptr<Mesh>& mesh, Ptr<Shader>& shader, Matrix* modelMatrix, int viewId)
{
	setWorldMatrice(*modelMatrix);

	bgfx::setVertexBuffer(0, mesh->m_vertexBuffer);
	bgfx::setIndexBuffer(mesh->m_indexBuffer);
	bgfx::submit(viewId, shader->m_program);
}

void Rendering::draw(Ptr<Mesh>& mesh, Matrix* modelMatrix)
{
	//m_gbufferShader->setTexture(0, ???);
	draw(mesh, m_gbufferShader, modelMatrix, RENDERVIEW_GBUFFER);
}

void Rendering::blit(uint view, bgfx::TextureHandle texture)
{
	auto textureFlags = 0 | BGFX_TEXTURE_MIN_POINT | BGFX_TEXTURE_MAG_POINT | BGFX_TEXTURE_MIP_POINT;

	bgfx::setTexture(0, m_texture0, texture, textureFlags);

	bgfx::setVertexBuffer(0, m_blitMesh->m_vertexBuffer);
	bgfx::setIndexBuffer(m_blitMesh->m_indexBuffer);
	bgfx::submit(view, m_blitShader->m_program);
}

void Rendering::setWorldMatrice(Matrix& modelMatrix)
{
	auto view = Camera::m_mainCamera->m_view;
	auto proj = Camera::m_mainCamera->m_projection;

	auto mat = modelMatrix * view * proj;
	mat.transpose();

	bgfx::setUniform(m_modelViewProjection, &mat);
}

void Rendering::setState(bool tristrip, bool msaa, bool uiRendering, bool debugLines, bool debugTriangles)
{
	if (debugLines)
	{
		bgfx::setState(0 
			| BGFX_STATE_RGB_WRITE
			| BGFX_STATE_PT_LINES
			| BGFX_STATE_LINEAA
			| BGFX_STATE_CULL_CW
			| BGFX_STATE_BLEND_ALPHA);
		return;
	}

	if (debugTriangles)
	{
		bgfx::setState(0 
			| BGFX_STATE_RGB_WRITE
			| BGFX_STATE_CULL_CCW
			| BGFX_STATE_BLEND_ALPHA);
		return;
	}

	if (uiRendering)
	{
		bgfx::setState(0
			| BGFX_STATE_RGB_WRITE
			| BGFX_STATE_ALPHA_WRITE
			| BGFX_STATE_BLEND_FUNC_SEPARATE(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA, BGFX_STATE_BLEND_ZERO, BGFX_STATE_BLEND_ONE)
			| BGFX_STATE_BLEND_EQUATION(BGFX_STATE_BLEND_EQUATION_ADD)
			& ~BGFX_STATE_DEPTH_TEST_LESS
		);

		return;
	}

	if (tristrip)
		bgfx::setState(0 | BGFX_STATE_DEFAULT | BGFX_STATE_PT_TRISTRIP);
	else
		bgfx::setState(0 | BGFX_STATE_DEFAULT);
}

void Rendering::dispose()
{
	Logger::logInfo("Unloading rendering pipeline");

	m_gbuffer->dispose();
	Logger::logInfo("Unloaded render buffers");
	
	m_blitMesh->dispose();

	m_blitShader->dispose();
	m_gbufferShader->dispose();
	m_deferredFinal->dispose();
	Logger::logInfo("Unloaded shaders");

	bgfx::destroy(m_modelViewProjection);
	bgfx::destroy(m_texture0);
	bgfx::destroy(m_texture1);
	bgfx::destroy(m_texture2);
	bgfx::destroy(m_texture3);
	Logger::logInfo("Unloaded uniforms");

	// suicide
	delete this;
}
