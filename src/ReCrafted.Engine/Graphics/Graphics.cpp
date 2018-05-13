// ReCrafted (c) 2016-2018 Always Too Late

#include "Graphics.h"

#include "Common/Display.h"
#include "Common/Profiler/Profiler.h"
#include "Common/Input/Input.h"
#include "Core/Logger.h"
#include "Core/Application.h"
#include "Game/Universe.h"
#include "Graphics/Camera.h"
#include "Graphics/RenderBuffer.h"
#include "Graphics/UI/UI.h"
#include "Graphics/WebUI/WebUI.h"

SINGLETON_IMPL(Graphics)

Graphics* g_rendererInstance;

void Graphics::loadInternalShaders()
{
    Logger::logInfo("Loading internal shaders");

    m_gbufferFillShader = Shader::loadShader("../assets/shaders/GBufferStandard.shader");
    m_gbufferCombine = Shader::loadShader("../assets/shaders/GBufferCombine.shader");
}

void Graphics::createRenderBuffers()
{
    Logger::logInfo("Creating render buffers");

    // create render buffer for geometry pass
    m_gbuffer = RenderBuffer::createRenderBuffer();
    m_gbuffer->begin();
    m_gbuffer->addTarget("ALBEDO", Renderer::TextureFormat::RGBA8);
    m_gbuffer->addTarget("[RGB]NORMALS, [A]AmbientOcculusion", Renderer::TextureFormat::RGBA8);
    m_gbuffer->end();
}

void Graphics::initializeRenderer()
{
    Logger::logInfo("Creating renderer with Direct3D11 API");

    // Initialize Renderer
    Renderer::Initialize(
        Renderer::RendererAPI::DirectX11,
        Renderer::Settings::Debug,
        Renderer::RenderFlags::_enum((Renderer::RenderFlags::Default) & ~Renderer::RenderFlags::VSync)
    );

    // Create Output
    m_window = Renderer::CreateWindowHandle(Platform::getCurrentWindow());
    m_frameBuffer = Renderer::GetWindowRenderBuffer(m_window);

    Logger::logInfo("Graphics initialized");
}

void Graphics::onInit()
{
    g_rendererInstance = this;

    // initialize renderer
    initializeRenderer();

    // create render buffers
    createRenderBuffers();

    // load all shaders
    loadInternalShaders();
}

void Graphics::onDispose()
{
    Logger::logInfo("Unloading rendering pipeline");

    m_gbuffer->dispose();
    Logger::logInfo("Unloaded render buffers");

    // dispose shaders
    m_gbufferFillShader->dispose();
    m_gbufferCombine->dispose();
    Logger::logInfo("Unloaded render shaders");

    // check resource leaks
    Logger::logInfo("Checking for resource leaks");
    IResource::checkLeaks();

    // shutdown renderer
    Renderer::Shutdown();

    Logger::logInfo("Graphics is down");
}

void Graphics::update()
{
}

void Graphics::render()
{
    Profiler::beginProfile("Render");
    {
        cvar clearColor = Renderer::Color{0.0f, 0.0f, 0.0f, 1.0f};

        cvar renderBuffer = m_gbuffer->m_renderBufferHandle;

        Renderer::ApplyWindow(m_window);
        Renderer::ApplyRenderBuffer(renderBuffer);
        Renderer::ClearRenderBuffer(renderBuffer, clearColor);
        Renderer::ClearRenderBuffer(m_frameBuffer, clearColor);

        // set default render stage
        setStage(RenderStage::Default);

        // begin rendering
        renderBegin();
        {
            // render world
            renderWorld();

            // render UI
            renderUI();
        }
        renderEnd(); // end rendering

        // next frame, wait vsync
        Renderer::Frame();
    }
    Profiler::endProfile();
}

void Graphics::resize(uint width, uint height)
{
    _ASSERT(Camera::m_mainCamera != nullptr);

    Display::set_Width(width);
    Display::set_Height(height);

    if (WebUI::getInstance())
        WebUI::getInstance()->resize(width, height);

    m_gbuffer->resize(width, height);
    Renderer::ResizeWindow(m_window, width, height);
}

void Graphics::renderBegin()
{
    // update main camera
    Camera::m_mainCamera->update();

    // set default shader
    setShader(m_gbufferFillShader);

    // set default matrix
    rvar mvpMatrix = Camera::getMainCamera()->get_viewProjection();
    Renderer::SetShaderValue(m_currentShader->m_shaderHandle, 0, 0, &mvpMatrix, sizeof(Matrix));

    var nearPlane = Camera::getMainCamera()->get_nearPlane();
    Renderer::SetShaderValue(m_currentShader->m_shaderHandle, 0, 1, &nearPlane, sizeof(float));

    var farPlane = Camera::getMainCamera()->get_farPlane();
    Renderer::SetShaderValue(m_currentShader->m_shaderHandle, 0, 2, &farPlane, sizeof(float));

    // bind gbuffer
    m_gbuffer->bind();
}

void Graphics::renderEnd()
{
    setStage(RenderStage::Default);

    if (Input::isKey(Key_F1))
    {
        Renderer::BlitTexture(m_frameBuffer, m_gbuffer->getTarget(0));
        // reset everything
        m_currentShader = nullptr;
        return;
    }

    if (Input::isKey(Key_F2))
    {
        Renderer::BlitTexture(m_frameBuffer, m_gbuffer->getTarget(1));
        // reset everything
        m_currentShader = nullptr;
        return;
    }

    // combine gbuffer

    // update shaders uniforms
    var lightdir = Vector3(0.39f, 0.9f, 0.13f);
    lightdir.normalize();
    m_gbufferCombine->setValue(0, &lightdir);

    // blit render textures using gbuffercombine shader
    rvar gbufferDescription = Renderer::GetRenderBufferDescription(m_gbuffer->m_renderBufferHandle);
    Renderer::BlitTextures(
        m_gbufferCombine->m_shaderHandle,
        m_frameBuffer,
        gbufferDescription.renderTextures.data(),
        static_cast<uint8_t>(gbufferDescription.renderTextures.size())
    );

    // reset everything
    m_currentShader = nullptr;
}

void Graphics::renderWorld()
{
    Profiler::beginProfile("Render World");
    {
        // render universe
        Universe::getInstance()->render();
    }
    Profiler::endProfile();
}

void Graphics::renderUI()
{
    Profiler::beginProfile("Render UI");
    {
        // set UI state
        setStage(RenderStage::DrawUI);

        // draw UI
        UI::m_instance->beginDraw(); // begin draw UI

        Profiler::beginProfile("UI Collect");
        {
            // render application UI
            Application::getInstance()->renderUI();

            // draw profiler debug screen
            Profiler::drawDebugScreen();
        }
        Profiler::endProfile();

        Profiler::beginProfile("UI Process");
        {
            UI::m_instance->endDraw(); // end draw UI
        }
        Profiler::endProfile();
    }
    Profiler::endProfile();

    Profiler::beginProfile("WebUI Render");
    {
        // set WebUI state
        setStage(RenderStage::DrawWebUI);

        WebUI::getInstance()->render();
    }
    Profiler::endProfile();
}

void Graphics::draw(Ref<Mesh>& mesh)
{
    if (!(RENDERER_CHECK_HANDLE(mesh->m_vertexBuffer)) || !(RENDERER_CHECK_HANDLE(mesh->m_indexBuffer)))
        return;

    Renderer::ApplyVertexBuffer(mesh->m_vertexBuffer);
    Renderer::ApplyIndexBuffer(mesh->m_indexBuffer);

    Renderer::DrawIndexed(mesh->m_indices_count);
}

void Graphics::setShader(Ref<Shader>& shader)
{
    if (m_currentShader != shader)
    {
        Renderer::ApplyShader(shader->m_shaderHandle, 0);
        m_currentShader = shader;
    }
}

void Graphics::setView(int viewId)
{
    m_viewId = viewId;
}

void Graphics::setMatrix(Matrix& mvpMatrix)
{
    Renderer::SetShaderValue(m_currentShader->m_shaderHandle, 0, 0, &mvpMatrix, sizeof(Matrix));
}

void Graphics::setStage(RenderStage::_enum stage)
{
    /*m_renderStage = stage;

    switch (stage)
    {
    case RenderStage::DrawUI:
    case RenderStage::DrawWebUI:
        bgfx::setState(0
            | BGFX_STATE_RGB_WRITE
            | BGFX_STATE_ALPHA_WRITE
            | BGFX_STATE_BLEND_FUNC_SEPARATE(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA, BGFX_STATE_BLEND_ZERO, BGFX_STATE_BLEND_ONE)
            | BGFX_STATE_BLEND_EQUATION(BGFX_STATE_BLEND_EQUATION_ADD)
            & ~BGFX_STATE_DEPTH_TEST_LESS
        );
        return;

    case RenderStage::DebugDrawLines:
        bgfx::setState(0
            | BGFX_STATE_RGB_WRITE
            | BGFX_STATE_PT_LINES
            | BGFX_STATE_LINEAA
            | BGFX_STATE_CULL_CW
            | BGFX_STATE_BLEND_ALPHA);
        return;

    case RenderStage::DebugDrawTriangles:
        bgfx::setState(0
            | BGFX_STATE_RGB_WRITE
            | BGFX_STATE_CULL_CCW
            | BGFX_STATE_BLEND_ALPHA);
        return;

    case RenderStage::Default:
    default:
        setView(1);
        bgfx::setState(0 | BGFX_STATE_DEFAULT);
        return;
    }*/
}
