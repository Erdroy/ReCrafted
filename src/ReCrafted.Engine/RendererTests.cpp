// ReCrafted (c) 2016-2018 Always Too Late
// ReSharper disable CppRedundantQualifier

#include <Windows.h>

#include "ReCraftedConfig.h"
#include "Platform/Platform.h"

#if GFXL_TEST

#include "Graphics/Renderer/GFXL/GFXL.hpp"

static bool m_running = true;

void initRendererTests()
{
    // Create application window
    //WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, nullptr, nullptr, LoadCursor(nullptr, IDC_ARROW), nullptr, nullptr, TEXT("GFXL Example"), nullptr };
    //RegisterClassEx(&wc);
    //auto hwnd = CreateWindow(TEXT("GFXL Example"), TEXT("GFXL Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 768, NULL, NULL, wc.hInstance, NULL);

    // Show the window
    //ShowWindow(hwnd, SW_SHOWDEFAULT);
    //UpdateWindow(hwnd);

    // Initialize GFXL
    GFXL::Initialize(
        GFXL::RendererAPI::DirectX11,
        GFXL::ResetFlags::VSync,
        GFXL::Settings::Debug
    );

    // Create Output
    cvar window = GFXL::CreateWindowHandle(Platform::getCurrentWindow());
    cvar frameBuffer = GFXL::GetWindowRenderBuffer(window);

    // Create simple shader
    cvar shader = GFXL::CreateShader("gfxlshaders/simpleShader.shader");
    cvar clearColor = GFXL::Color{ 0.0f, 0.2f, 0.4f, 1.0f };

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
        GFXL::ApplyWindow(window);
        GFXL::ApplyRenderBuffer(frameBuffer);
        GFXL::ClearRenderBuffer(frameBuffer, clearColor);

        // Set shader as current
        GFXL::ApplyShader(shader, 0);
        //GFXL::SetShaderValue(0, 1.0f);

        // Push frame
        GFXL::Frame();
    }

    GFXL::DestroyShader(shader);
    GFXL::DestroyWindow(window);
    GFXL::Shutdown();

    //UnregisterClassW(TEXT("GFXL Example"), nullptr);
}
#endif