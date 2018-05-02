// ReCrafted (c) 2016-2018 Always Too Late
// ReSharper disable CppRedundantQualifier

#include "ReCraftedConfig.h"

#if RENDERER_TEST

#include <Windows.h>

#include "Platform/Platform.h"
#include "Core/Math/Vector2.h"
#include "Core/Math/Vector3.h"
#include "Core/Math/Vector4.h"
#include "Core/Logger.h"

#include "Graphics/Renderer/Renderer.hpp"

static bool m_running = true;

void initRendererTests()
{
    // Initialize Renderer
    Renderer::Initialize(
        Renderer::RendererAPI::DirectX11,
        Renderer::RenderFlags::DepthTest | Renderer::RenderFlags::DepthStencil,
        Renderer::Settings::Debug
    );

    // Create Output
    cvar window = Renderer::CreateWindowHandle(Platform::getCurrentWindow());
    cvar frameBuffer = Renderer::GetWindowRenderBuffer(window);

    // Create simple shader
    cvar shader = Renderer::CreateShader("shaders/simpleShader.shader");
    cvar clearColor = Renderer::Color{ 0.0f, 0.2f, 0.4f, 1.0f };

    struct { Vector3 pos; Vector2 uv; } simpleMeshVertices[3];
    simpleMeshVertices[0].pos.x = -1.0f;
    simpleMeshVertices[0].pos.y = -1.0f;
    simpleMeshVertices[0].pos.z = 0.0f;
    simpleMeshVertices[0].uv.x = 0.0f;
    simpleMeshVertices[0].uv.y = 0.0f;

    simpleMeshVertices[1].pos.x = 0.0f;
    simpleMeshVertices[1].pos.y = 1.0f;
    simpleMeshVertices[1].pos.z = 0.0f;
    simpleMeshVertices[1].uv.x = 0.5f;
    simpleMeshVertices[1].uv.y = 1.0f;

    simpleMeshVertices[2].pos.x = 1.0f;
    simpleMeshVertices[2].pos.y = -1.0f;
    simpleMeshVertices[2].pos.z = 0.0f;
    simpleMeshVertices[2].uv.x = 1.0f;
    simpleMeshVertices[2].uv.y = 0.0f;

    uint simpleMeshIndices[3];
    simpleMeshIndices[0] = 0;
    simpleMeshIndices[1] = 1;
    simpleMeshIndices[2] = 2;

    cvar vbMemory = Renderer::Allocate(3 * sizeof(Vector3) + 3 * sizeof(Vector2));
    cvar ibMemory = Renderer::Allocate(3 * sizeof(uint));
    memcpy(vbMemory, simpleMeshVertices, 3 * sizeof(Vector3) + 3 * sizeof(Vector2));
    memcpy(ibMemory, simpleMeshIndices, 3 * sizeof(uint));

    cvar triangleVB = Renderer::CreateVertexBuffer(3, sizeof Vector3 + sizeof Vector2, vbMemory);
    cvar triangleIB = Renderer::CreateIndexBuffer(3, ibMemory);

    simpleMeshVertices[0].pos.z = 0.0f;
    simpleMeshVertices[1].pos.z = 1.0f;
    simpleMeshVertices[2].pos.z = 2.0f;
    simpleMeshVertices[0].pos.x = -1.0f;
    simpleMeshVertices[1].pos.x = -1.0f;
    simpleMeshVertices[2].pos.x = 1.0f;
    cvar vbMemory1 = Renderer::Allocate(3 * sizeof(Vector3) + 3 * sizeof(Vector2));
    memcpy(vbMemory1, simpleMeshVertices, 3 * sizeof(Vector3) + 3 * sizeof(Vector2));

    cvar triangleVB1 = Renderer::CreateVertexBuffer(3, sizeof Vector3 + sizeof Vector2, vbMemory1);

    cvar textureDataSize = 32 * 32 * 4;
    cvar textureData = (byte*)Renderer::Allocate(textureDataSize);

    for (var y = 0; y < 32; y++)
    {
        for (var x = 0; x < 32 * 4; x += 4)
        {
            textureData[x + y * 32 * 4 + 0] = byte(float(rand()) / float(RAND_MAX) * 255);
            textureData[x + y * 32 * 4 + 1] = byte(float(rand()) / float(RAND_MAX) * 255);
            textureData[x + y * 32 * 4 + 2] = byte(float(rand()) / float(RAND_MAX) * 255);
            textureData[x + y * 32 * 4 + 3] = 255;
        }
    }
    cvar texture = Renderer::CreateTexture2D(32, 32, Renderer::TextureFormat::RGBA8, textureData, textureDataSize);


    Renderer::TextureFormat::_enum textureFormats[1] = { Renderer::TextureFormat::RGBA8 };
    cvar renderBuffer = Renderer::CreateRenderBuffer(1280, 720, textureFormats, 1, Renderer::TextureFormat::D32);

    Renderer::ResizeRenderBuffer(renderBuffer, 3840, 2160);

    MSG msg = {};

    // Main loop
    while (m_running)
    {
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        var start = Platform::getMiliseconds();

        // Set output as current
        Renderer::ApplyWindow(window);

        Renderer::ApplyRenderBuffer(renderBuffer);
        Renderer::ClearRenderBuffer(renderBuffer, clearColor);

        // Set shader value
        var newValue = Vector4(1.0f, 0.5f, 0.0f, 1.0f);
        Renderer::SetShaderValue(shader, 0, 0, &newValue, sizeof (Vector4));

        // Set shader as current
        Renderer::ApplyShader(shader, 0);
        Renderer::ApplyTexture2D(texture, 0);

        // Set triangle VB as current
        Renderer::ApplyVertexBuffer(triangleVB1);
        Renderer::ApplyIndexBuffer(triangleIB);

        // Draw triangle
        Renderer::DrawIndexed(3);

        // Set triangle VB as current
        Renderer::ApplyVertexBuffer(triangleVB);
        Renderer::ApplyIndexBuffer(triangleIB);

        // Draw triangle
        Renderer::DrawIndexed(3);

        // blit renderbuffer[0] into framebuffer
        Renderer::Blit(frameBuffer, renderBuffer.renderTextures[0]);

        // Push frame
        Renderer::Frame();

        var time = Platform::getMiliseconds() - start;
        //Logger::log("Frame render time: {0}", time);
    }

    Renderer::DestroyRenderBuffer(renderBuffer);
    Renderer::DestroyVertexBuffer(triangleVB);
    Renderer::DestroyShader(shader);
    Renderer::DestroyWindow(window);
    Renderer::Shutdown();
}
#endif