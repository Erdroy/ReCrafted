// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "ImGuiManager.h"
#include "Common/Logger.h"
#include "Core/Display.h"
#include "Rendering/Renderer/Renderer.h"
#include "Rendering/Renderer/RHI/RHIContext.h"

#include <imgui.h>

#include "Impl/imgui_impl_win32.h"
#include "Impl/imgui_impl_dx11.h"
#include "Profiler/Profiler.h"

IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void ImGuiManager::Initialize()
{
    // Get renderer context
    Renderer::RHIContext rendererContext;
    Renderer::GetContext(&rendererContext);

    // Initialize ImGUI
    Logger::Log("Initializing ImGUI {0}", IMGUI_VERSION);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    const auto newFont = io.Fonts->AddFontFromFileTTF("../Fonts/Lato-Regular.ttf", 14.0f);
    io.Fonts->AddFontDefault(newFont->ConfigData);

    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

#ifdef WIN32
    ImGui_ImplWin32_Init(Platform::GetCurrentWindow());

    // TODO: TODO: Select proper renderer
    ImGui_ImplDX11_Init(static_cast<ID3D11Device*>(rendererContext.device), static_cast<ID3D11DeviceContext*>(rendererContext.deviceContext));

    // Add event handler
    Platform::AddEventProcessor(reinterpret_cast<Platform::EventDelegate>(&ImGui_ImplWin32_WndProcHandler));
#endif

    // Setup style
    ImGui::StyleColorsDark();
    auto& style = ImGui::GetStyle();
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
}

void ImGuiManager::Shutdown()
{
    // shutdown ImGUI
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiManager::Render()
{
}

void ImGuiManager::BeginRender()
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

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

void ImGuiManager::EndRender()
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

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
}
