// ReCrafted (c) 2016-2018 Always Too Late
// ReSharper disable CppRedundantQualifier

#include <Windows.h>

#include "ReCraftedConfig.h"
#include "Platform/Platform.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"

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

    Vector3 simpleMeshVertices[3];
    simpleMeshVertices[0].x = -1.0f;
    simpleMeshVertices[0].y = -1.0f;
    simpleMeshVertices[0].z = 1.0f;

    simpleMeshVertices[1].x = 0.0f;
    simpleMeshVertices[1].y = 1.0f;
    simpleMeshVertices[1].z = 1.0f;

    simpleMeshVertices[2].x = 1.0f;
    simpleMeshVertices[2].y = -1.0f;
    simpleMeshVertices[2].z = 1.0f;

    uint simpleMeshIndices[3];
    simpleMeshIndices[0] = 0;
    simpleMeshIndices[1] = 1;
    simpleMeshIndices[2] = 2;

    cvar vbMemory = Renderer::Allocate(3 * sizeof(Vector3));
    memcpy(vbMemory, simpleMeshVertices, 3 * sizeof(Vector3));

    cvar ibMemory = Renderer::Allocate(3 * sizeof(uint));
    memcpy(ibMemory, simpleMeshIndices, 3 * sizeof(uint));

    cvar triangleVB = Renderer::CreateVertexBuffer(3, sizeof Vector3, vbMemory);
    cvar triangleIB = Renderer::CreateIndexBuffer(3, ibMemory);

    cvar texture = Renderer::CreateTexture2D(800, 600, Renderer::TextureFormat::RGBA8, nullptr, 0u);

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

        // Set shader value
        var newValue = Vector4(1.0f, 0.5f, 0.0f, 1.0f);
        Renderer::SetShaderValue(shader, 0, 0, &newValue, sizeof (Vector4));

        // Set shader as current
        Renderer::ApplyShader(shader, 0);

        // Set triangle VB as current
        Renderer::ApplyVertexBuffer(triangleVB);
        Renderer::ApplyIndexBuffer(triangleIB);

        // Draw triangle
        Renderer::DrawIndexed(3);

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