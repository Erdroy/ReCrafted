// ReCrafted (c) 2016-2018 Always Too Late

#include "Graphics.h"

#include "Common/Display.h"
#include "Common/Profiler/Profiler.h"
#include "Common/Input/Input.h"
#include "Core/Logger.h"
#include "Core/Action.h"
#include "Core/Application.h"
#include "Graphics/Camera.h"
#include "Graphics/RenderBuffer.h"
#include "UI/UI.h"
#include "WebUI/WebUI.h"
#include "DebugDraw.h"
#include "RenderTasks/ScreenshotTask.h"
#include "Rendering/PostProcessing/PPVignette.h"
#include "Rendering/PostProcessing/PPSSAO.h"
#include "Content/ContentManager.h"

#include "imgui.h"
#include "impl/imgui_impl_win32.h"
#include "impl/imgui_impl_dx11.h"
#include "Renderer/RHI/RHIContext.h"

SINGLETON_IMPL(Graphics)

Graphics* g_rendererInstance;

void Graphics::LoadInternalShaders()
{
    Logger::LogInfo("Loading internal shaders");

    m_gbufferFillShader = ContentManager::LoadAsset<Shader>("Shaders/StandardShader");
    m_gbufferCombine = ContentManager::LoadAsset<Shader>("Shaders/GBufferCombine");
}

void Graphics::CreateRenderBuffers()
{
    Logger::LogInfo("Creating render buffers");

    // create render buffer for geometry pass
    m_gbuffer = RenderBuffer::CreateRenderBuffer();
    m_gbuffer->Begin();
    m_gbuffer->AddTarget("ALBEDO", Renderer::TextureFormat::RGBA8);
    m_gbuffer->AddTarget("[RGB]NORMALS, [A]AmbientOcculusion", Renderer::TextureFormat::RGBA8);
    m_gbuffer->AddTarget("[RGB]LIGHT, [A]EMISSION", Renderer::TextureFormat::RGBA8);
    m_gbuffer->End();

    m_frameTexture = Renderer::CreateRenderTexture(Display::GetWidth(), Display::GetHeight(), Renderer::TextureFormat::RGBA8);
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

    // Get renderer context
    Renderer::RHIContext rendererContext;
    Renderer::GetContext(&rendererContext);

    // Create Output
    m_window = Renderer::CreateWindowHandle(Platform::GetCurrentWindow());
    m_frameBuffer = Renderer::GetWindowRenderBuffer(m_window);

    // Initialize rendering
    m_rendering.reset(new Rendering());
    m_rendering->Initialize();

    for(var i = 0; i < 16; i ++)
        m_currentTextures.Add(nullptr);

    // Set renderer callbacks
    Renderer::AddOnPresentBeginEvent(Action<void>::New<Graphics, &Graphics::RenderWebUI>(this));
    Renderer::AddOnPresentBeginEvent(Action<void>::New<Graphics, &Graphics::RenderImGUI>(this));

    // Initialize ImGUI
    Logger::Log("Initializing ImGUI {0}", IMGUI_VERSION);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    cvar newFont = io.Fonts->AddFontFromFileTTF("../assets/fonts/Lato-Regular.ttf", 14.0f);
    io.Fonts->AddFontDefault(newFont->ConfigData);

    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    ImGui_ImplWin32_Init(Platform::GetCurrentWindow());
    ImGui_ImplRenderer_Init(rendererContext.device, rendererContext.deviceContext);

    // Setup style
    ImGui::StyleColorsDark();
    rvar style = ImGui::GetStyle();
    style.ScrollbarRounding = 2.0f;
    style.WindowPadding = ImVec2(4.0f, 4.0f);

    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.28f, 0.28f, 0.28f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.0f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);

    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);

    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.28f, 0.28f, 0.28f, 1.0f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);

    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.28f, 0.28f, 0.28f, 1.0f);

    Logger::LogInfo("Graphics initialized");
}

void Graphics::InitializePostProcessing()
{
    m_vignette.reset(new PPVignette);
    m_ssao.reset(new PPSSAO);
    Rendering::RegisterPostProcessing(m_vignette.get());
    //Rendering::RegisterPostProcessing(m_ssao.get());
}

void Graphics::UpdateDefaultConstants(const Matrix& mvp)
{
    // Set MVP matrix
    m_currentShader->SetValue(0, const_cast<Matrix*>(&mvp));

    // Set inverted view matrix
    var invView = Camera::GetMainCamera()->GetView();
    invView.Invert();
    invView.Transpose();
    m_currentShader->SetValue(1, &invView);

    // Set view info
    cvar projection = Camera::GetMainCamera()->GetProjection();
    Vector4 viewInfo;
    viewInfo.x = Camera::GetMainCamera()->GetNearPlane();
    viewInfo.y = Camera::GetMainCamera()->GetFarPlane();
    viewInfo.z = 1.0f / projection.m11;
    viewInfo.w = 1.0f / projection.m22;

    m_currentShader->SetValue(2, &viewInfo);

    // Set screen size
    Vector2 screenSize;
    screenSize.x = static_cast<float>(Display::GetWidth());
    screenSize.y = static_cast<float>(Display::GetHeight());

    m_currentShader->SetValue(3, &screenSize);

    // Set camera position vector
    var cameraPosition = Camera::GetMainCamera()->GetPosition();
    m_currentShader->SetValue(4, &cameraPosition);

    // Set light direction vector
    var lightdir = Vector3(0.2f, 0.3f, 0.1f);
    lightdir.Normalize();
    m_currentShader->SetValue(5, &lightdir);

    // Set light direction vector
    var ambientLight = Vector3(0.35f, 0.35f, 0.35f);
    m_currentShader->SetValue(6, &ambientLight);

    // apply shader changes
    Renderer::ApplyShader(m_currentShader->m_shaderHandle, 0);
}

void Graphics::BlitFrameBuffer()
{
    // blit into framebuffer
    Renderer::BlitTexture(m_frameBuffer, m_frameTexture);
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

    // Initialize post processing
    InitializePostProcessing();
}

void Graphics::OnDispose()
{
    Logger::LogInfo("Unloading rendering pipeline");

    // Dispose rendering
    SafeDispose(m_rendering);

    SafeDispose(m_gbuffer);
    Logger::LogInfo("Unloaded render buffers");

    // shutdown ImGUI
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // Dispose shaders
    Object::Destroy(m_gbufferFillShader);
    Object::Destroy(m_gbufferCombine);
    Logger::LogInfo("Unloaded render shaders");

    // Shutdown renderer
    Renderer::Shutdown();

    Logger::LogInfo("Graphics is down");
}

void Graphics::Update()
{
    UpdateImGUI();
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
        Renderer::ClearRenderTexture(m_frameTexture, clearColor);

        // begin rendering
        RenderBegin();
        {
            // Render geometry
            m_rendering->RenderGeometry();

            // Render shadows
            m_rendering->RenderShadows();
        }
        RenderEnd(); // end rendering

        // Render post processing
        //m_rendering->RenderPostProcessing(m_frameTexture, m_gbuffer->GetTarget(1), m_gbuffer->GetDepthBuffer());

        // Blit into framebuffer
        BlitFrameBuffer();

        // Set frame buffer as current
        //Renderer::ApplyRenderBuffer(m_frameBuffer);

        // Render debug draw
        //RenderDebugDraw();

        // Render UI
        RenderUI();

        // Render (fullscreen) WebUIViews
        RenderWebUIViews();

        Profiler::BeginProfile("Renderer::Frame");
        // next frame, wait vsync
        Renderer::Frame();
        Profiler::EndProfile();
    }
    Profiler::EndProfile();
}

void Graphics::Resize(uint width, uint height)
{
    ASSERT(Camera::m_mainCamera != nullptr);

    ImGui_ImplDX11_InvalidateDeviceObjects();

    Display::SetWidth(width);
    Display::SetHeight(height);

    if (WebUI::GetInstance())
        WebUI::GetInstance()->Resize(width, height);

    m_gbuffer->Resize(width, height);
    Renderer::ResizeWindow(m_window, width, height);

    if(RENDERER_CHECK_HANDLE(m_frameTexture))
    {
        Renderer::DestroyTexture2D(m_frameTexture);
        m_frameTexture = Renderer::CreateTexture2D(width, height, 0, 0, Renderer::TextureFormat::RGBA8, nullptr, 0u, true);
    }

    ImGui_ImplDX11_CreateDeviceObjects();
}

void Graphics::RenderBegin()
{
    // set default render stage
    SetStage(RenderStage::Default);

    // Update main camera
    Camera::m_mainCamera->Update();

    // set default shader
    SetShader(m_gbufferFillShader);

    // Update
    UpdateDefaultConstants(Camera::GetMainCamera()->GetViewProjection());

    if (Input::IsKey(Key_F1))
        Renderer::SetFlag(Renderer::RenderFlags::DrawWireFrame, true);

    // Clean texture cache
    for(var i = 0u; i < m_currentTextures.Count(); i ++)
    {
        m_currentTextures[i] = nullptr;
    }

    // bind gbuffer
    m_gbuffer->Bind();
}

void Graphics::RenderEnd()
{
    SetStage(RenderStage::Default);

    if (Input::IsKey(Key_F1))
    {
        Renderer::SetFlag(Renderer::RenderFlags::DrawWireFrame, false);
        Renderer::BlitTexture(m_frameTexture, m_gbuffer->GetTarget(0));
        // reset everything
        m_currentShader = nullptr;
        return;
    }

    if (Input::IsKey(Key_F2))
    {
        Renderer::BlitTexture(m_frameTexture, m_gbuffer->GetTarget(0));
        // reset everything
        m_currentShader = nullptr;
        return;
    }

    if (Input::IsKey(Key_F3))
    {
        Renderer::BlitTexture(m_frameTexture, m_gbuffer->GetTarget(1));
        // reset everything
        m_currentShader = nullptr;
        return;
    }

    if (Input::IsKey(Key_F4))
    {
        Renderer::BlitTexture(m_frameTexture, m_gbuffer->GetTarget(2));
        // reset everything
        m_currentShader = nullptr;
        return;
    }

    if (Input::IsKey(Key_F5))
    {
        Renderer::BlitTexture(m_frameTexture, m_gbuffer->GetDepthBuffer());
        // reset everything
        m_currentShader = nullptr;
        return;
    }

    // Set shader
    SetShader(m_gbufferCombine);

    // Update
    UpdateDefaultConstants(Camera::GetMainCamera()->GetViewProjection());

    // blit render textures using gbuffercombine shader
    rvar gbufferDescription = Renderer::RenderBufferHandlePool::GetHandleDescription(m_gbuffer->m_renderBufferHandle);

    // Build render target list (Color, Normal, Depth)
    var renderTargets = std::vector<Renderer::Texture2DHandle>(gbufferDescription.renderTextures);
    renderTargets.push_back(m_gbuffer->GetDepthBuffer());

    Renderer::BlitTextures(
        m_frameTexture,
        renderTargets.data(),
        static_cast<uint8_t>(renderTargets.size()),
        m_gbufferCombine->m_shaderHandle
    );

    // reset everything
    m_currentShader = nullptr;
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

void Graphics::UpdateImGUI()
{
    // Start the Dear ImGui frame
    // ImGUI rendering is not multi-threaded,
    // and it is going to be rendered on before preset event
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(Display::GetWidth(), Display::GetHeight()));

    ImGui::PushStyleColor(ImGuiCol_WindowBg, 0x00000000);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
}

void Graphics::RenderWebUI()
{
    Profiler::BeginProfile("Render WebUI");
    {
        WebUI::GetInstance()->Render();
    }
    Profiler::EndProfile();
}

void Graphics::RenderWebUIViews()
{
    Profiler::BeginProfile("Render WebUIViews");
    {
        // set WebUI state
        SetStage(RenderStage::DrawWebUI);

        WebUI::GetInstance()->RenderViews();
    }
    Profiler::EndProfile();
}

void Graphics::RenderImGUI()
{
    Profiler::BeginProfile("Render ImGui");
    
    // Get renderer context
    Renderer::RHIContext rendererContext;
    Renderer::GetContext(&rendererContext);

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    ImGui::End();

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    ImGui::Render();
    ImGUI_ImplDX11_SetRenderTarget(rendererContext.windows[1].backBuffer);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    Profiler::EndProfile();
}

void Graphics::Draw(const RefPtr<Mesh>& mesh, Shader* shader)
{
    if (!(RENDERER_CHECK_HANDLE(mesh->m_vertexBuffer)) || !(RENDERER_CHECK_HANDLE(mesh->m_indexBuffer)))
        return;

    if(shader != m_currentShader)
        SetShader(shader);

    Draw(mesh);
}

void Graphics::Draw(const RefPtr<Mesh>& mesh)
{
    ASSERT(m_currentShader);

    Renderer::ApplyVertexBuffer(mesh->m_vertexBuffer);
    Renderer::ApplyIndexBuffer(mesh->m_indexBuffer);

    Renderer::DrawIndexed(mesh->m_indices_count);
}

void Graphics::SetShader(const RefPtr<Shader>& shader)
{
    SetShader(shader.get());
}

void Graphics::SetShader(Shader* shader)
{
    m_currentShader = shader;
    Renderer::ApplyShader(shader->m_shaderHandle, 0);
    UpdateDefaultConstants(Camera::GetMainCamera()->GetViewProjection());
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

void Graphics::SetTexture(uint slot, Texture* texture2d)
{
    if (m_currentTextures[slot] != texture2d)
    {
        Renderer::ApplyTexture2D(texture2d->GetHandle(), slot);
        m_currentTextures[slot] = texture2d;
    }
}

void Graphics::SetTextureArray(uint slot, Texture** textureArray, uint8_t textureCount)
{
    // Copy textures
    Renderer::Texture2DHandle textures[32]; // Max 32 textures per array

    for(var i = 0u; i < textureCount; i++)
    {
        cvar texture = textureArray[i];

        if(texture)
            textures[i] = texture->GetHandle();
    }

    SetTextureArray(slot, textures, textureCount);
}

void Graphics::SetTextureArray(uint slot, Renderer::Texture2DHandle* textureArray, uint8_t textureCount)
{
    Renderer::ApplyTextureArray2D(textureArray, slot, textureCount);
}

void Graphics::Screenshot(Text fileName)
{
    // Create texture
    cvar texture = Renderer::CreateTexture2D(Display::GetWidth(), Display::GetHeight(), 0, Renderer::TextureFormat::RGBA8, Renderer::TextureType::Staging);

    // Capture
    Renderer::CaptureFrame(texture);

    // Do Screenshot rendertask
    Renderer::ExecuteTask(new ScreenshotTask(texture, fileName));

    // Delete texture
    Renderer::DestroyTexture2D(texture);
}
