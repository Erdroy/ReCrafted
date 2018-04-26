// ReCrafted (c) 2016-2018 Always Too Late
// ReSharper disable CppRedundantQualifier

#include <Windows.h>

#include "ReCraftedConfig.h"
#include "Platform/Platform.h"

#if RENDERER_TEST

#include "Graphics/Renderer/Renderer.hpp"

static bool m_running = true;

void initRendererTests()
{
    // Create application window
    //WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, nullptr, nullptr, LoadCursor(nullptr, IDC_ARROW), nullptr, nullptr, TEXT("Renderer Example"), nullptr };
    //RegisterClassEx(&wc);
    //auto hwnd = CreateWindow(TEXT("Renderer Example"), TEXT("Renderer Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 768, NULL, NULL, wc.hInstance, NULL);

    // Show the window
    //ShowWindow(hwnd, SW_SHOWDEFAULT);
    //UpdateWindow(hwnd);

    // Initialize Renderer
    Renderer::Initialize(
        Renderer::RendererAPI::DirectX11,
        Renderer::ResetFlags::VSync,
        Renderer::Settings::Debug
    );

    // Create Output
    cvar window = Renderer::CreateWindowHandle(Platform::getCurrentWindow());
    cvar frameBuffer = Renderer::GetWindowRenderBuffer(window);

    // Create simple shader
    cvar shader = Renderer::CreateShader("shaders/simpleShader.shader");
    cvar clearColor = Renderer::Color{ 0.0f, 0.2f, 0.4f, 1.0f };

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (m_running)
    {
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Set output as current
        Renderer::ApplyWindow(window);
        Renderer::ApplyRenderBuffer(frameBuffer);
        Renderer::ClearRenderBuffer(frameBuffer, clearColor);

        // Set shader as current
        Renderer::ApplyShader(shader, 0);
        //Renderer::SetShaderValue(0, 1.0f);

        // Push frame
        Renderer::Frame();
    }

    Renderer::DestroyShader(shader);
    Renderer::DestroyWindow(window);
    Renderer::Shutdown();

    //UnregisterClassW(TEXT("Renderer Example"), nullptr);
}
#endif