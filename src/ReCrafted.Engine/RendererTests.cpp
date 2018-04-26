// ReCrafted (c) 2016-2018 Always Too Late
// ReSharper disable CppRedundantQualifier

#include <Windows.h>

#include "Core/Math/Vector3.h"
#include "ReCraftedConfig.h"
#include "Platform/Platform.h"

#if RENDERER_TEST

#include "Graphics/Renderer/Renderer.hpp"

static bool m_running = true;

void initRendererTests()
{
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

    Vector3 simpleMesh[3];
    simpleMesh[0].x = -1.0f;
    simpleMesh[0].y = -1.0f;
    simpleMesh[0].z = 1.0f;

    simpleMesh[1].x = 0.0f;
    simpleMesh[1].y = 1.0f;
    simpleMesh[1].z = 1.0f;

    simpleMesh[2].x = 1.0f;
    simpleMesh[2].y = -1.0f;
    simpleMesh[2].z = 1.0f;

    var triangleVB = Renderer::CreateVertexBuffer(3, sizeof Vector3, (byte*)&simpleMesh, false);

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

        // Set triangle VB as current
        Renderer::ApplyVertexBuffer(triangleVB);

        // Draw triangle
        Renderer::Draw(3);

        // Push frame
        Renderer::Frame();
    }

    Renderer::DestroyVertexBuffer(triangleVB);
    Renderer::DestroyShader(shader);
    Renderer::DestroyWindow(window);
    Renderer::Shutdown();

    //UnregisterClassW(TEXT("Renderer Example"), nullptr);
}
#endif