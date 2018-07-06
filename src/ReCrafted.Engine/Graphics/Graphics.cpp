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
#include "DebugDraw.h"

SINGLETON_IMPL(Graphics)

Graphics* g_rendererInstance;

void Graphics::LoadInternalShaders()
{
    Logger::LogInfo("Loading internal shaders");

    m_gbufferFillShader = Shader::LoadShader("../assets/shaders/GBufferStandard.shader");
    m_gbufferCombine = Shader::LoadShader("../assets/shaders/GBufferCombine.shader");
}

void Graphics::CreateRenderBuffers()
{
    Logger::LogInfo("Creating render buffers");

    // create render buffer for geometry pass
    m_gbuffer = RenderBuffer::CreateRenderBuffer();
    m_gbuffer->Begin();
    m_gbuffer->AddTarget("ALBEDO", Renderer::TextureFormat::RGBA8);
    m_gbuffer->AddTarget("[RGB]NORMALS, [A]AmbientOcculusion", Renderer::TextureFormat::RGBA8);
    m_gbuffer->End();
}

void Graphics::InitializeRenderer()
{
    Logger::LogInfo("Creating renderer with Direct3D11 API");

    // Initialize Renderer
    Renderer::Initialize(
        Renderer::RendererAPI::DirectX11,
        Renderer::Settings::Debug,
        Renderer::RenderFlags::_enum(Renderer::RenderFlags::Default)
    );

    Renderer::SetFlag(Renderer::RenderFlags::VSync, false);

    // Create Output
    m_window = Renderer::CreateWindowHandle(Platform::GetCurrentWindow());
    m_frameBuffer = Renderer::GetWindowRenderBuffer(m_window);

    // Set renderer callbacks
    var onFramePresent = MakeDelegate(Graphics::OnFramePresent);
    Renderer::AddOnPresentCallback(onFramePresent);

    Logger::LogInfo("Graphics initialized");
}

void Graphics::OnInit()
{
    g_rendererInstance = this;

    // initialize renderer
    InitializeRenderer();

    // create render buffers
    CreateRenderBuffers();

    // load all shaders
    LoadInternalShaders();
}

void Graphics::OnDispose()
{
    Logger::LogInfo("Unloading rendering pipeline");

    m_gbuffer->Dispose();
    Logger::LogInfo("Unloaded render buffers");

    // Dispose shaders
    m_gbufferFillShader->Dispose();
    m_gbufferCombine->Dispose();
    Logger::LogInfo("Unloaded render shaders");

    // check resource leaks
    Logger::LogInfo("Checking for resource leaks");
    IResource::CheckLeaks();

    // Shutdown renderer
    Renderer::Shutdown();

    Logger::LogInfo("Graphics is down");
}

void Graphics::Update()
{
}

void Graphics::Render()
{
    Profiler::BeginProfile("Rendering");
    {
        cvar clearColor = Renderer::Color{0.0f, 0.0f, 0.0f, 1.0f};

        cvar renderBuffer = m_gbuffer->m_renderBufferHandle;

        Renderer::ApplyWindow(m_window);
        Renderer::ApplyRenderBuffer(renderBuffer);
        Renderer::ClearRenderBuffer(renderBuffer, clearColor);
        Renderer::ClearRenderBuffer(m_frameBuffer, clearColor);

        // set default render stage
        SetStage(RenderStage::Default);

        // begin rendering
        RenderBegin();
        {
            // render world
            RenderWorld();
        }
        RenderEnd(); // end rendering

        // render debug draw
        RenderDebugDraw();

        // render UI
        RenderUI();

        // next frame, wait vsync
        Renderer::Frame();
    }
    Profiler::EndProfile();
}

void Graphics::Resize(uint width, uint height)
{
    ASSERT(Camera::m_mainCamera != nullptr);

    Display::SetWidth(width);
    Display::SetHeight(height);

    if (WebUI::GetInstance())
        WebUI::GetInstance()->Resize(width, height);

    m_gbuffer->Resize(width, height);
    Renderer::ResizeWindow(m_window, width, height);
}

void Graphics::RenderBegin()
{
    // Update main camera
    Camera::m_mainCamera->Update();

    // set default shader
    SetShader(m_gbufferFillShader);

    // set default matrix
    rvar mvpMatrix = Camera::GetMainCamera()->GetViewProjection();
    m_currentShader->SetValue(0, &mvpMatrix);

    var nearPlane = Camera::GetMainCamera()->GetNearPlane();
    m_currentShader->SetValue(1, &nearPlane);

    var farPlane = Camera::GetMainCamera()->GetFarPlane();
    m_currentShader->SetValue(2, &farPlane);

    // apply shader changes
    Renderer::ApplyShader(m_gbufferFillShader->m_shaderHandle, 0);

    if (Input::IsKey(Key_F1))
        Renderer::SetFlag(Renderer::RenderFlags::DrawWireFrame, true);

    // bind gbuffer
    m_gbuffer->Bind();
}

void Graphics::RenderEnd()
{
    SetStage(RenderStage::Default);

    if (Input::IsKey(Key_F1))
    {
        Renderer::SetFlag(Renderer::RenderFlags::DrawWireFrame, false);
        Renderer::BlitTexture(m_frameBuffer, m_gbuffer->GetTarget(0));
        // reset everything
        m_currentShader = nullptr;
        return;
    }

    if (Input::IsKey(Key_F2))
    {
        Renderer::BlitTexture(m_frameBuffer, m_gbuffer->GetTarget(0));
        // reset everything
        m_currentShader = nullptr;
        return;
    }

    if (Input::IsKey(Key_F3))
    {
        Renderer::BlitTexture(m_frameBuffer, m_gbuffer->GetTarget(1));
        // reset everything
        m_currentShader = nullptr;
        return;
    }

    if (Input::IsKey(Key_F4))
    {
        Renderer::BlitTexture(m_frameBuffer, m_gbuffer->GetDepthBuffer());
        // reset everything
        m_currentShader = nullptr;
        return;
    }

    // combine gbuffer

    // Update shaders uniforms
    var lightdir = Vector3(0.39f, 0.9f, 0.13f);
    lightdir.Normalize();
    m_gbufferCombine->SetValue(0, &lightdir);

    var cameraPosition = Camera::GetMainCamera()->GetPosition();
    m_gbufferCombine->SetValue(1, &cameraPosition);

    Vector4 viewInfo = {};
    viewInfo.x = Camera::GetMainCamera()->GetNearPlane();
    viewInfo.y = Camera::GetMainCamera()->GetFarPlane();

    m_gbufferCombine->SetValue(2, &viewInfo);

    // blit render textures using gbuffercombine shader
    rvar gbufferDescription = Renderer::GetRenderBufferDescription(m_gbuffer->m_renderBufferHandle);

    // Build render target list (Color, Normal, Depth)
    var renderTargets = std::vector<Renderer::Texture2DHandle>(gbufferDescription.renderTextures);
    renderTargets.push_back(m_gbuffer->GetDepthBuffer());

    Renderer::BlitTextures(
        m_gbufferCombine->m_shaderHandle,
        m_frameBuffer,
        renderTargets.data(),
        static_cast<uint8_t>(renderTargets.size())
    );

    // reset everything
    m_currentShader = nullptr;
}

void Graphics::RenderWorld()
{
    Profiler::BeginProfile("Render World");
    {
        // render universe
        Universe::GetInstance()->Render();
    }
    Profiler::EndProfile();
}

void Graphics::RenderDebugDraw()
{
    Profiler::BeginProfile("Render Debug");
    {
        DebugDraw::GetInstance()->Render();
    }
    Profiler::EndProfile();
}

void Graphics::RenderUI()
{
    Profiler::BeginProfile("Render UI");
    {
        // set UI state
        SetStage(RenderStage::DrawUI);

        // draw UI
        UI::m_instance->BeginDraw(); // begin draw UI

        Profiler::BeginProfile("UI Collect");
        {
            // render application UI
            Application::GetInstance()->RenderUI();

            // draw profiler debug screen
            Profiler::GetInstance()->DrawDebugScreen();
        }
        Profiler::EndProfile();

        Profiler::BeginProfile("UI Process");
        {
            UI::m_instance->EndDraw(); // end draw UI
        }
        Profiler::EndProfile();
    }
    Profiler::EndProfile();
}

void Graphics::OnFramePresent(void* unused)
{
    Profiler::BeginProfile("Render WebUI");
    {
        // set WebUI state
        SetStage(RenderStage::DrawWebUI);

        WebUI::GetInstance()->Render();
    }
    Profiler::EndProfile();
}

void Graphics::Draw(RefPtr<Mesh>& mesh)
{
    if (!(RENDERER_CHECK_HANDLE(mesh->m_vertexBuffer)) || !(RENDERER_CHECK_HANDLE(mesh->m_indexBuffer)))
        return;

    Renderer::ApplyVertexBuffer(mesh->m_vertexBuffer);
    Renderer::ApplyIndexBuffer(mesh->m_indexBuffer);

    Renderer::DrawIndexed(mesh->m_indices_count);
}

void Graphics::SetShader(RefPtr<Shader>& shader)
{
    if (m_currentShader != shader)
    {
        Renderer::ApplyShader(shader->m_shaderHandle, 0);
        m_currentShader = shader;
    }
}

void Graphics::SetStage(RenderStage::_enum stage)
{
    m_renderStage = stage;

    switch (stage)
    {
    case RenderStage::DrawUI:
    case RenderStage::DrawWebUI:
        Renderer::SetFlag(Renderer::RenderFlags::DepthTest, false);
        Renderer::SetFlag(Renderer::RenderFlags::DepthStencil, false);
        Renderer::SetFlag(Renderer::RenderFlags::DrawLineLists, false);
        Renderer::SetFlag(Renderer::RenderFlags::RenderOverlay, true); // temporary simplified blend-states TODO: Expose Renderer's blend state creation
        return;

    case RenderStage::DebugDrawLines:
        Renderer::SetFlag(Renderer::RenderFlags::DepthTest, false);
        Renderer::SetFlag(Renderer::RenderFlags::DepthStencil, false);
        Renderer::SetFlag(Renderer::RenderFlags::DrawLineLists, true);
        Renderer::SetFlag(Renderer::RenderFlags::RenderOverlay, true);
        return;

    case RenderStage::DebugDrawTriangles:
        Renderer::SetFlag(Renderer::RenderFlags::DepthTest, false);
        Renderer::SetFlag(Renderer::RenderFlags::DepthStencil, false);
        Renderer::SetFlag(Renderer::RenderFlags::DrawLineLists, false);
        Renderer::SetFlag(Renderer::RenderFlags::RenderOverlay, true);
        return;

    case RenderStage::Default:
    default:
    {
        Renderer::SetFlags(Renderer::RenderFlags::_enum(Renderer::RenderFlags::Default));
        Renderer::SetFlag(Renderer::RenderFlags::VSync, false);
    }
    }
}
