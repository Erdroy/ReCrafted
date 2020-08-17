// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "DirectX11.h"

#include "RHIDirectX11.h"
#include "Common/Platform/Platform.h"

#if RENDERER_RHI_D3D11

#include "Rendering/Renderer/Renderer.h"
#include "Rendering/Renderer/RendererConfig.h"
#include "Profiler/Profiler.h"

#include "../RHIContext.h"

#include "Internal/RHIDirectX11_RenderBuffer.h"
#include "Internal/RHIDirectX11_Shader.h"

#include <nlohmann/json.hpp>
#include <thread>
#include <vector>
#include <string>
#include <fstream>

namespace Renderer
{
    using json = nlohmann::json;

    namespace RHI
    {
        class WorkerThreadInstance;

        const int FrameBufferCount = 1;

        struct WindowDesc
        {
        public:
            RenderBufferHandle renderBuffer = {};
            IDXGISwapChain* swapChain = nullptr;
            HWND hwnd = nullptr;

        public:
            void GetSize(uint* width, uint* height) const
            {
                ASSERT(hwnd);

                RECT windowRect = {};
                GetClientRect(hwnd, &windowRect);

                *width = windowRect.right - windowRect.left;
                *height = windowRect.bottom - windowRect.top;
            }
        };

        struct IndexBufferDesc
        {
        public:
            ID3D11Buffer* buffer = nullptr;
            bool is32bit = true;
        public:
            void Dispose()
            {
                SafeRelease(buffer);
                buffer = nullptr;
            }
        };

        struct Texture2DDesc
        {
        public:
            ID3D11Texture2D* texture = nullptr;
            ID3D11ShaderResourceView* srv = nullptr;
            ID3D11RenderTargetView* rtv = nullptr;
            ID3D11DepthStencilView* dsv = nullptr;
            
            uint16_t width = 0u;
            uint16_t height = 0u;
            uint16_t pitch = 0u;
            uint16_t mips = 0u;
            TextureFormat::_enum format = TextureFormat::Unknown;

        public:
            void Dispose()
            {
                SafeRelease(texture);
                SafeRelease(srv);
                SafeRelease(rtv);
                SafeRelease(dsv);
            }
        };

        // == static ==
        RHIDirectX11* RHIDirectX11::m_instance;

        // == common ==
        volatile bool m_running;
        int m_workerThreadCount;
        int m_msaaSampleCount;
        Settings::_enum m_settings;
        RenderFlags::_enum m_renderFlags;
        RenderFlags::_enum m_defaultFlags;


        // == stats ==
        uint32_t m_apiCalls = 0u;
        uint32_t m_drawCalls = 0u;
        uint32_t m_commandCount = 0u;

        uint32_t m_verticesDrawn = 0u;
        uint32_t m_indicesDrawn = 0u;


        // last stats (for GetRenderStatistics, to make sure, that outputed stats are the total ones)
        uint32_t m_lastApiCalls = 0u;
        uint32_t m_lastDrawCalls = 0u;
        uint32_t m_lastCommandCount = 0u;

        uint32_t m_lastVerticesDrawn = 0u;
        uint32_t m_lastIndicesDrawn = 0u;


        // == events ==
        HANDLE m_workerFinishEvents[RENDERER_MAX_RENDER_THREADS];
        HANDLE m_workerFrameEvents[RENDERER_MAX_RENDER_THREADS];


        // == worker-related stuff ==
        WorkerThreadInstance* m_workerThreads[RENDERER_MAX_RENDER_THREADS] = {};


        // == resources ==
        WindowDesc m_windows[RENDERER_MAX_WINDOWS] = {};
        IDXGISwapChain1* m_swapChains[RENDERER_MAX_WINDOWS] = {};
        RHIDirectX11_Shader* m_shaders[RENDERER_MAX_SHADER_PROGRAMS] = {};
        RHIDirectX11_RenderBuffer* m_renderBuffers[RENDERER_MAX_RENDER_BUFFERS] = {};
        ID3D11Buffer* m_vertexBuffers[RENDERER_MAX_VERTEX_BUFFERS] = {};
        IndexBufferDesc m_indexBuffers[RENDERER_MAX_INDEX_BUFFERS] = {};
        Texture2DDesc m_textures[RENDERER_MAX_TEXTURES2D] = {};

        ID3D11DepthStencilState* m_depthStencilState;
        ID3D11DepthStencilState* m_depthStencilState_Disabled;
        ID3D11DepthStencilState* m_depthStencilState_NoDepth;
        ID3D11DepthStencilState* m_depthStencilState_NoStencil;

        ID3D11RasterizerState* m_rasterizerState;
        ID3D11RasterizerState* m_rasterizerState_WireFrame;

        ID3D11BlendState* m_blendState_Overlay;


        // == d3d11 resources ==
        ID3D11Debug* m_debug = nullptr;
        ID3D11Device* m_device = nullptr;
        ID3D11DeviceContext* m_deviceContext = nullptr;


        // == pointer-only resources (DO NOT RELEASE!) ==
        IDXGISwapChain1* m_swapChain = nullptr;

#pragma region WorkerThread impl
        class WorkerThreadInstance
        {
            friend class RHIDirectX11;

        private:
            HANDLE m_fenceEvent = {};

            uint32_t m_commandCount = 0u;

            ID3D11DeviceContext* m_context = nullptr;

        private:
            RHIDirectX11_Shader* m_currentShader = nullptr;
            RHIDirectX11_RenderBuffer* m_currentRenderBuffer = nullptr;
            WindowDesc* m_currentWindow = nullptr;

            CommandList m_commandList;

        public:
            std::thread thread;

            int threadId = 0;
            int frameCount = 0;

        public:
            void WaitForPreviousFrame();
            void WaitForExit();

            void InitializeWorker();
            void WorkerThread();

            void ResetFlags();

            void PrepareFrame();
            void ProcessFrame();
            void ExecuteCommandList();
            void ExecuteCommand(CommandHeader::_enum header, uint32_t* position);

            void Cleanup();

        public:
            FORCEINLINE void Execute_QueueFree(Command_QueueFree* command);

            FORCEINLINE void Execute_ExecuteTask(Command_ExecuteTask* command);

            FORCEINLINE void Execute_CaptureFrame(Command_CaptureFrame* command);

            FORCEINLINE void Execute_SetFlag(Command_SetFlag* command);
            FORCEINLINE void Execute_SetFlags(Command_SetFlags* command);

            FORCEINLINE void Execute_Draw(Command_Draw* command);
            FORCEINLINE void Execute_DrawIndexed(Command_DrawIndexed* command);

            FORCEINLINE void Execute_ResizeFrameBuffer(Command_ResizeFrameBuffer* command);

            FORCEINLINE void Execute_CreateRenderBuffer(Command_CreateRenderBuffer* command);
            FORCEINLINE void Execute_ResizeRenderBuffer(Command_ResizeRenderBuffer* command);
            FORCEINLINE void Execute_ClearRenderBuffer(Command_ClearRenderBuffer* command);
            FORCEINLINE void Execute_ApplyRenderBuffer(Command_ApplyRenderBuffer* command);
            FORCEINLINE void Execute_DestroyRenderBuffer(Command_DestroyRenderBuffer* command);

            FORCEINLINE void Execute_CreateShader(Command_CreateShader* command);
            FORCEINLINE void Execute_ApplyShader(Command_ApplyShader* command);
            FORCEINLINE void Execute_DestroyShader(Command_DestroyShader* command);

            FORCEINLINE void Execute_CreateVertexBuffer(Command_CreateVertexBuffer* command);
            FORCEINLINE void Execute_ApplyVertexBuffer(Command_ApplyVertexBuffer* command);
            FORCEINLINE void Execute_UpdateVertexBuffer(Command_UpdateVertexBuffer* command);
            FORCEINLINE void Execute_DestroyVertexBuffer(Command_DestroyVertexBuffer* command);

            FORCEINLINE void Execute_CreateIndexBuffer(Command_CreateIndexBuffer* command);
            FORCEINLINE void Execute_ApplyIndexBuffer(Command_ApplyIndexBuffer* command);
            FORCEINLINE void Execute_UpdateIndexBuffer(Command_UpdateIndexBuffer* command);
            FORCEINLINE void Execute_DestroyIndexBuffer(Command_DestroyIndexBuffer* command);

            FORCEINLINE void Execute_CreateTexture2D(Command_CreateTexture2D* command);
            FORCEINLINE void Execute_ApplyTexture2D(Command_ApplyTexture2D* command);
            FORCEINLINE void Execute_ApplyTextureArray2D(Command_ApplyTextureArray2D* command);
            FORCEINLINE void Execute_ResizeTexture2D(Command_ResizeTexture2D* command);
            FORCEINLINE void Execute_UpdateTexture2D(Command_UpdateTexture2D* command);
            FORCEINLINE void Execute_UpdateViewTexture2D(Command_UpdateViewTexture2D* command);
            FORCEINLINE void Execute_DestroyTexture2D(Command_DestroyTexture2D* command);

            FORCEINLINE void Execute_ApplyRenderTexture2D(Command_ApplyRenderTexture2D* command);
            FORCEINLINE void Execute_ClearRenderTexture2D(Command_ClearRenderTexture2D* command);
        };

        void WorkerThreadInstance::WaitForPreviousFrame()
        {
            // There is no previous frame, yet, so we don't have to wait for the frame. YEY.
        }

        void WorkerThreadInstance::WaitForExit()
        {
            WaitForSingleObject(m_workerFinishEvents[threadId], INFINITE);

            if (thread.joinable())
                thread.join();
        }

        void WorkerThreadInstance::InitializeWorker()
        {
            if (m_settings & Settings::SingleThreaded)
            {
                m_context = m_deviceContext;
            }
            else
            {
                DX_CALL(m_device->CreateDeferredContext(0, &m_context));
            }

            m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
            ASSERT(m_fenceEvent != nullptr);

            // we are done with worker thread setup now
        }

        void WorkerThreadInstance::WorkerThread()
        {
            Platform::SetThreadName("RenderThread");

            //ScriptingEngine::AttachCurrentThread();

            // Wait for main thread to signal ready
            WaitForSingleObject(m_workerFrameEvents[threadId], INFINITE);

            while (m_running)
            {
                // Process frame now
                ProcessFrame();

                // Signal main thread that we have done our job here
                SetEvent(m_workerFinishEvents[threadId]);

                // Wait for main thread to signal ready
                WaitForSingleObject(m_workerFrameEvents[threadId], INFINITE);

                frameCount++;
            }

            //ScriptingEngine::DetachCurrentThread();
        }

        void WorkerThreadInstance::ResetFlags()
        {
            m_renderFlags = m_defaultFlags;

            // Setup default states (reset)
            for (auto i = 0u; 1u << i < RenderFlags::Count; i++)
            {
                const auto flag = RenderFlags::_enum(1u << i);  // NOLINT
                const auto flagState = GetFlag(flag);

                Command_SetFlag command = {};
                command.flag = flag;
                command.value = flagState;
                command.size = 0;

                Execute_SetFlag(&command);
            }
        }

        void WorkerThreadInstance::PrepareFrame()
        {
            m_context->ClearState();

            // setup all flags to default
            ResetFlags();
        }

        void WorkerThreadInstance::ProcessFrame()
        {
            WaitForPreviousFrame();
            PrepareFrame();
            ExecuteCommandList();
        }

        void WorkerThreadInstance::ExecuteCommandList()
        {
            if (m_commandCount == 0u)
                return;

            // Execute command list
            auto position = 0u;
            for (auto i = 0u; i < m_commandCount; i++)
            {
                auto header = m_commandList.Read_CommandHeader(&position);
                position -= sizeof header;

                ExecuteCommand(header, &position);
            }
        }

        void WorkerThreadInstance::ExecuteCommand(CommandHeader::_enum header, uint32_t* position)
        {
#define DEFINE_COMMAND_EXECUTOR(name) \
            case CommandHeader::name: { \
                auto command = m_commandList.ReadCommand<Command_##name>(position);\
                Execute_##name(&command);\
                break; \
		    }

            switch (header)
            {
            case CommandHeader::ApplyWindow:
                {
                    const auto command = m_commandList.ReadCommand<Command_ApplyWindow>(position);
                    const auto idx = command.window.idx;

                    // set window's swapchain as current
                    m_swapChain = m_swapChains[idx];
                    break;
                }
            case CommandHeader::DestroyWindow:
                {
                    const auto command = m_commandList.ReadCommand<Command_DestroyWindow>(position);
                    const auto idx = command.window.idx;

                    SafeRelease(m_swapChains[idx]);

                    const auto windowDesc = WindowHandlePool::GetHandleDescription(command.window);
                    const auto renderBufferIdx = windowDesc.renderBuffer.idx;

                    SafeRelease(m_renderBuffers[renderBufferIdx]);

                    break;
                }
            case CommandHeader::SetShaderValue:
                {
                    float data[16];
                    const auto command = m_commandList.ReadCommand<Command_SetShaderValue>(position);
                    m_commandList.Read(data, position, command.dataSize);

                    auto& shader = m_shaders[command.shader.idx];
                    shader->SetValue(command.bufferId, command.fieldId, data, command.dataSize);
                    break;
                }
            case CommandHeader::SetShaderValues:
                {
                    const auto command = m_commandList.ReadCommand<Command_SetShaderValues>(position);

                    auto& shader = m_shaders[command.shader.idx];
                    shader->SetValues(command.bufferId, command.memory, command.memorySize, command.memoryOffset);
                    break;
                }
            DEFINE_COMMAND_EXECUTOR(QueueFree);

            DEFINE_COMMAND_EXECUTOR(ExecuteTask);

            DEFINE_COMMAND_EXECUTOR(CaptureFrame);

            DEFINE_COMMAND_EXECUTOR(SetFlag);
            DEFINE_COMMAND_EXECUTOR(SetFlags);

            DEFINE_COMMAND_EXECUTOR(Draw);
            DEFINE_COMMAND_EXECUTOR(DrawIndexed);

            DEFINE_COMMAND_EXECUTOR(ResizeFrameBuffer);

            DEFINE_COMMAND_EXECUTOR(CreateRenderBuffer);
            DEFINE_COMMAND_EXECUTOR(ResizeRenderBuffer);
            DEFINE_COMMAND_EXECUTOR(ClearRenderBuffer);
            DEFINE_COMMAND_EXECUTOR(ApplyRenderBuffer);
            DEFINE_COMMAND_EXECUTOR(DestroyRenderBuffer);

            DEFINE_COMMAND_EXECUTOR(CreateShader);
            DEFINE_COMMAND_EXECUTOR(ApplyShader);
            DEFINE_COMMAND_EXECUTOR(DestroyShader);

            DEFINE_COMMAND_EXECUTOR(CreateVertexBuffer);
            DEFINE_COMMAND_EXECUTOR(ApplyVertexBuffer);
            DEFINE_COMMAND_EXECUTOR(UpdateVertexBuffer);
            DEFINE_COMMAND_EXECUTOR(DestroyVertexBuffer);

            DEFINE_COMMAND_EXECUTOR(CreateIndexBuffer);
            DEFINE_COMMAND_EXECUTOR(ApplyIndexBuffer);
            DEFINE_COMMAND_EXECUTOR(UpdateIndexBuffer);
            DEFINE_COMMAND_EXECUTOR(DestroyIndexBuffer);

            DEFINE_COMMAND_EXECUTOR(CreateTexture2D);
            DEFINE_COMMAND_EXECUTOR(ApplyTexture2D);
            DEFINE_COMMAND_EXECUTOR(ApplyTextureArray2D);
            DEFINE_COMMAND_EXECUTOR(ResizeTexture2D);
            DEFINE_COMMAND_EXECUTOR(UpdateTexture2D);
            DEFINE_COMMAND_EXECUTOR(UpdateViewTexture2D);
            DEFINE_COMMAND_EXECUTOR(DestroyTexture2D);

            DEFINE_COMMAND_EXECUTOR(ApplyRenderTexture2D);
            DEFINE_COMMAND_EXECUTOR(ClearRenderTexture2D);

            case CommandHeader::Unknown:
            case CommandHeader::Count:
            default:
                break;
            }
        }

        void WorkerThreadInstance::Cleanup()
        {
            m_commandList.Destroy();

            if (!(m_settings & Settings::SingleThreaded))
                SafeRelease(m_context);
        }

        void WorkerThreadInstance::Execute_QueueFree(Command_QueueFree* command)
        {
            // Free memory when valid
            if(command->memory != nullptr)
                Free(command->memory);
        }

        void WorkerThreadInstance::Execute_ExecuteTask(Command_ExecuteTask* command)
        {
            // Set device and context
            command->task->device = m_device;
            command->task->context = m_context;

            // Execute task
            command->task->execute();

            // Delete task
            delete command->task;
        }

        void WorkerThreadInstance::Execute_CaptureFrame(Command_CaptureFrame* command)
        {
            const auto outputTexture = m_textures[command->targetTexture.idx].texture;
            ASSERT(outputTexture != nullptr);

            const auto renderBuffer = m_renderBuffers[m_currentWindow->renderBuffer.idx];
            const auto renderTarget = renderBuffer->GetRTVs()[0];

            ASSERT(renderTarget != nullptr);

            ID3D11Resource* backBuffer;
            renderTarget->GetResource(&backBuffer);

            ASSERT(backBuffer != nullptr);

            m_deviceContext->CopyResource(outputTexture, backBuffer);
        }

        void WorkerThreadInstance::Execute_SetFlag(Command_SetFlag* command)
        {
            if(command->size != 0) // when not called by ResetFlags
            {
                if(command->value)
                {
                    m_renderFlags = RenderFlags::_enum(m_renderFlags | command->flag);
                }
                else
                {
                    m_renderFlags = RenderFlags::_enum(m_renderFlags & ~command->flag);
                }
            }

            switch(command->flag)
            {
            case RenderFlags::DrawLineLists:
            {
                m_context->IASetPrimitiveTopology(command->value ? D3D11_PRIMITIVE_TOPOLOGY_LINELIST : D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                ADD_APICALL();
                break;
            }

            case RenderFlags::DrawWireFrame:
            {
                m_context->RSSetState(command->value ? m_rasterizerState_WireFrame : m_rasterizerState);
                ADD_APICALL();
                break;
            }

            case RenderFlags::DepthTest:
            case RenderFlags::DepthStencil:
            {
                const auto stencilEnabled = m_renderFlags & RenderFlags::DepthStencil;
                const auto depthEnabled = m_renderFlags & RenderFlags::DepthTest;

                if (stencilEnabled && depthEnabled)
                {
                    m_context->OMSetDepthStencilState(m_depthStencilState, 1u);
                    ADD_APICALL();
                }
                else if(stencilEnabled)
                {
                    m_context->OMSetDepthStencilState(m_depthStencilState_NoDepth, 1u);
                    ADD_APICALL();
                }
                else if (depthEnabled)
                {
                    m_context->OMSetDepthStencilState(m_depthStencilState_NoStencil, 1u);
                    ADD_APICALL();
                }
                else
                {
                    m_context->OMSetDepthStencilState(m_depthStencilState_Disabled, 1u);
                    ADD_APICALL();
                }

                break;
            }

            case RenderFlags::RenderOverlay:
            {
                m_context->OMSetBlendState(command->value ? m_blendState_Overlay : nullptr, nullptr, 0xFFFFFFFF);
                ADD_APICALL();
                break;
            }

            case RenderFlags::AnisotropicX2:
            case RenderFlags::AnisotropicX4:
            case RenderFlags::AnisotropicX8:
            case RenderFlags::AnisotropicX16:

            case RenderFlags::MSAAx2:
            case RenderFlags::MSAAx4:
            case RenderFlags::VSync:
            case RenderFlags::TripleBuffered:
            case RenderFlags::None:
            case RenderFlags::Count:
            default: 
                break;
            }
        }

        void WorkerThreadInstance::Execute_SetFlags(Command_SetFlags* command)
        {
            m_renderFlags = command->flags;
            ResetFlags();
        }
#pragma endregion

        // ========== WorkerThreadInstance::Executors ==========
#pragma region WorkerThread command execution implementation
        void WorkerThreadInstance::Execute_Draw(Command_Draw* command)
        {
            m_verticesDrawn += command->vertexCount;

            m_context->Draw(command->vertexCount, 0);
            ADD_APICALL();
            ADD_DRAWCALL();
        }

        void WorkerThreadInstance::Execute_DrawIndexed(Command_DrawIndexed* command)
        {
            m_indicesDrawn += command->indexCount;

            m_context->DrawIndexed(command->indexCount, 0, 0);
            ADD_APICALL();
            ADD_DRAWCALL();
        }

        void WorkerThreadInstance::Execute_ResizeFrameBuffer(Command_ResizeFrameBuffer* command)
        {
            auto& renderBuffer = m_renderBuffers[command->handle.idx];
            const auto swapChain = m_swapChains[command->windowHandle.idx];

            ASSERT(renderBuffer != nullptr);
            ASSERT(swapChain != nullptr);

            const auto bufferFormat = m_settings & Settings::BGRAFrameBuffer
                                    ? DXGI_FORMAT_B8G8R8A8_UNORM
                                    : DXGI_FORMAT_R8G8B8A8_UNORM;

            // Destroy render targets
            const auto rtvs = renderBuffer->GetRTVs();
            for (auto i = 0; i < FrameBufferCount; i ++)
            {
                SafeRelease(rtvs[i]);
            }

            // Destroy depth buffer
            if (renderBuffer->GetDSV())
                renderBuffer->GetDSV()->Release();

            DX_CALL(swapChain->ResizeBuffers(3, command->width, command->height, bufferFormat, 0u));

            ID3D11RenderTargetView* renderTargets[FrameBufferCount] = {};
            for (auto i = 0; i < FrameBufferCount; i++)
            {
                ID3D11Resource* resource = nullptr;
                DX_CALL(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&resource)));

                DX_CALL(m_device->CreateRenderTargetView(resource, nullptr, &renderTargets[i]));
                SafeRelease(resource);
            }

            // WARNING: No depth stencil view for back buffers!
            // TODO: Create depth stencil view

            renderBuffer->SetSize(command->width, command->height);
            renderBuffer->SetRTVs(renderTargets);
        }

        void WorkerThreadInstance::Execute_CreateShader(Command_CreateShader* command)
        {
            const auto shaderIdx = command->shader.idx;

            std::ifstream input(command->fileName);
            std::string content((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));

            auto jsonData = json::parse(content);
            m_shaders[shaderIdx] = RHIDirectX11_Shader::Create(m_device, jsonData);

        }
        
        void WorkerThreadInstance::Execute_ApplyShader(Command_ApplyShader* command)
        {
            auto shaderIdx = command->shader.idx;
            auto shader = m_shaders[shaderIdx];

            shader->Bind(m_context, command->passId);

            m_currentShader = shader;
        }

        void WorkerThreadInstance::Execute_DestroyShader(Command_DestroyShader* command)
        {
            auto shaderIdx = command->shader.idx;
            SafeRelease(m_shaders[shaderIdx]);
        }

        void WorkerThreadInstance::Execute_CreateVertexBuffer(Command_CreateVertexBuffer* command)
        {
            auto& buffer = m_vertexBuffers[command->handle.idx];
            ASSERT(buffer == nullptr);

            // create buffer description
            D3D11_BUFFER_DESC desc = {};
            desc.Usage = command->dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_IMMUTABLE;
            desc.ByteWidth = command->vertexSize * command->vertexCount;
            desc.StructureByteStride = command->vertexSize;
            desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            desc.CPUAccessFlags = command->dynamic ? D3D11_CPU_ACCESS_WRITE : 0;

            // create optional initial buffer data
            D3D11_SUBRESOURCE_DATA subresource_data = {};
            subresource_data.pSysMem = command->memory;
            subresource_data.SysMemPitch = 0;
            subresource_data.SysMemSlicePitch = 0;

            // create d3d11 buffer
            DX_CALL(m_device->CreateBuffer(&desc, command->memory ? &subresource_data : nullptr, &buffer));
        }

        void WorkerThreadInstance::Execute_ApplyVertexBuffer(Command_ApplyVertexBuffer* command)
        {
            auto& buffer = m_vertexBuffers[command->handle.idx];
            ASSERT(buffer != nullptr);

            ID3D11Buffer* buffers[] = {buffer};

            uint offset = 0u;
            uint stride = m_currentShader->GetStride();
            m_context->IASetVertexBuffers(0, 1, buffers, &stride, &offset);
            ADD_APICALL();
        }

        void WorkerThreadInstance::Execute_UpdateVertexBuffer(Command_UpdateVertexBuffer* command)
        {
            auto& buffer = m_vertexBuffers[command->handle.idx];
            ASSERT(buffer != nullptr);

            D3D11_MAPPED_SUBRESOURCE res = {};
            DX_CALL(m_context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res));

            const auto ptr = static_cast<byte*>(res.pData) + command->memoryOffset;
            memcpy(ptr, command->memory, command->memorySize);

            m_context->Unmap(buffer, 0);
            ADD_APICALL();
        }

        void WorkerThreadInstance::Execute_DestroyVertexBuffer(Command_DestroyVertexBuffer* command)
        {
            auto& buffer = m_vertexBuffers[command->handle.idx];
            ASSERT(buffer != nullptr);

            SafeRelease(buffer);

            VertexBufferHandlePool::FreeHandle(command->handle);
        }

        void WorkerThreadInstance::Execute_CreateIndexBuffer(Command_CreateIndexBuffer* command)
        {
            auto& buffer = m_indexBuffers[command->handle.idx];
            ASSERT(buffer.buffer == nullptr);

            buffer.is32bit = command->indexSize == 32;

            const auto bufferSizeBytes = buffer.is32bit ? 4 : 2;

            // create buffer description
            D3D11_BUFFER_DESC desc = {};
            desc.Usage = command->dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_IMMUTABLE;
            desc.ByteWidth = bufferSizeBytes * command->indexCount;
            desc.StructureByteStride = bufferSizeBytes;
            desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            desc.CPUAccessFlags = command->dynamic ? D3D11_CPU_ACCESS_WRITE : 0;

            // create optional initial buffer data
            D3D11_SUBRESOURCE_DATA subresource_data = {};
            subresource_data.pSysMem = command->memory;
            subresource_data.SysMemPitch = 0;
            subresource_data.SysMemSlicePitch = 0;

            // create d3d11 buffer
            DX_CALL(m_device->CreateBuffer(&desc, command->memory ? &subresource_data : nullptr, &buffer.buffer));
        }

        void WorkerThreadInstance::Execute_ApplyIndexBuffer(Command_ApplyIndexBuffer* command)
        {
            auto& buffer = m_indexBuffers[command->handle.idx];
            ASSERT(buffer.buffer != nullptr);

            m_context->IASetIndexBuffer(buffer.buffer, buffer.is32bit ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT,
                                        0u);
            ADD_APICALL();
        }

        void WorkerThreadInstance::Execute_UpdateIndexBuffer(Command_UpdateIndexBuffer* command)
        {
            auto& buffer = m_indexBuffers[command->handle.idx];
            ASSERT(buffer.buffer != nullptr);

            D3D11_MAPPED_SUBRESOURCE res;
            DX_CALL(m_context->Map(buffer.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res));

            const auto ptr = static_cast<byte*>(res.pData) + command->memoryOffset;
            memcpy(ptr, command->memory, command->memorySize);

            m_context->Unmap(buffer.buffer, 0);
            ADD_APICALL();
        }

        void WorkerThreadInstance::Execute_DestroyIndexBuffer(Command_DestroyIndexBuffer* command)
        {
            auto& buffer = m_indexBuffers[command->handle.idx];
            ASSERT(buffer.buffer != nullptr);

            buffer.Dispose();

            IndexBufferHandlePool::FreeHandle(command->handle);
        }

        void WorkerThreadInstance::Execute_CreateRenderBuffer(Command_CreateRenderBuffer* command)
        {
            auto& buffer = m_renderBuffers[command->handle.idx];
            ASSERT(buffer == nullptr);

            ASSERT(command->texturesCount > 0);
            ASSERT(command->width >= 16);
            ASSERT(command->height >= 16);

            // select rtv's
            std::vector<ID3D11RenderTargetView*> rtvs = {};
            for (auto i = 0; i < command->texturesCount; i ++)
            {
                const auto texture = m_textures[command->renderTargets[i].idx];
                ASSERT(texture.rtv != nullptr);
                rtvs.push_back(texture.rtv);
            }

            // select dsv if enabled
            ID3D11DepthStencilView* dsv = nullptr;
            if (command->createDepthStencil)
            {
                dsv = m_textures[command->depthTarget.idx].dsv;
                ASSERT(dsv != nullptr);
            }

            // create render buffer
            m_renderBuffers[command->handle.idx] = RHIDirectX11_RenderBuffer::Create(
                m_device, command->width, command->height, command->texturesCount, rtvs.data(), dsv);
        }

        void WorkerThreadInstance::Execute_ResizeRenderBuffer(Command_ResizeRenderBuffer* command)
        {
            auto& buffer = m_renderBuffers[command->handle.idx];
            ASSERT(buffer != nullptr);

            ASSERT(command->width >= 16);
            ASSERT(command->height >= 16);

            // set size
            buffer->SetSize(command->width, command->height);

            // rebind all textures
            // select rtv's
            std::vector<ID3D11RenderTargetView*> rtvs = {};
            for (auto i = 0u; i < command->texturesCount; i++)
            {
                const auto texture = m_textures[command->renderTargets[i].idx];
                ASSERT(texture.rtv != nullptr);
                rtvs.push_back(texture.rtv);
            }
            buffer->SetRTVs(rtvs.data());

            if (RENDERER_CHECK_HANDLE(command->depthTarget))
                buffer->SetDSV(m_textures[command->depthTarget.idx].dsv);
        }

        void WorkerThreadInstance::Execute_ClearRenderBuffer(Command_ClearRenderBuffer* command)
        {
            auto& buffer = m_renderBuffers[command->handle.idx];
            ASSERT(buffer != nullptr);

            // TODO: check if we can pass frame index (is it back buffer - render buffer)

            buffer->Clear(m_context, command->color, command->depth);
        }

        void WorkerThreadInstance::Execute_ApplyRenderBuffer(Command_ApplyRenderBuffer* command)
        {
            auto& buffer = m_renderBuffers[command->handle.idx];
            ASSERT(buffer != nullptr);

            // TODO: check if we can pass frame index (is it back buffer - render buffer)

            buffer->Bind(m_context);

            m_currentRenderBuffer = buffer;
        }

        void WorkerThreadInstance::Execute_DestroyRenderBuffer(Command_DestroyRenderBuffer* command)
        {
            auto& buffer = m_renderBuffers[command->handle.idx];
            ASSERT(buffer != nullptr);

            SafeRelease(buffer);
        }

        void WorkerThreadInstance::Execute_CreateTexture2D(Command_CreateTexture2D* command)
        {
            auto& texture = m_textures[command->handle.idx];
            ASSERT(texture.texture == nullptr);

            const auto mipLevels = command->mipLevels == 0 ? 1 : command->mipLevels;
            const auto createDepthBuffer = 
                command->textureFormat == TextureFormat::D16 ||
                command->textureFormat == TextureFormat::D24S8 ||
                command->textureFormat == TextureFormat::D32F;

            const auto rt = command->renderTarget || createDepthBuffer;

            D3D11_TEXTURE2D_DESC textureDesc = {};
            textureDesc.ArraySize = 1;
            textureDesc.Width = command->width;
            textureDesc.Height = command->height;
            textureDesc.MipLevels = command->generateMips ? 0 : mipLevels;
            textureDesc.MiscFlags = command->generateMips ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;
            textureDesc.SampleDesc.Count = rt ? m_msaaSampleCount : 1;

            if (createDepthBuffer)
            {
                textureDesc.Format = DGXI_TextureFormats[command->textureFormat][0];
                textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
            }
            else
            {
                textureDesc.Format = DGXI_TextureFormats[command->textureFormat][0];
                textureDesc.BindFlags = rt || command->generateMips ? 
                    D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE : D3D11_BIND_SHADER_RESOURCE;
            }

            if (command->textureType == TextureType::Staging)
            {
                textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
                textureDesc.Usage = D3D11_USAGE_STAGING;
                textureDesc.BindFlags = 0;
            }
            else
            {
                textureDesc.CPUAccessFlags = 0;
                textureDesc.Usage = D3D11_USAGE_DEFAULT;
            }

            DX_CALL(m_device->CreateTexture2D(&textureDesc, nullptr, &texture.texture));

            if(command->memory)
            {
                const auto dxgiFormat = DGXI_TextureFormats[command->textureFormat][0];
                auto memory = static_cast<uint8_t*>(command->memory);

                if(command->generateMips)
                {
                    size_t size;
                    size_t rowPitch;
                    size_t rowNum;

                    DXGICalculateSurfaceInfo(command->width, command->height, dxgiFormat, &size, &rowPitch, &rowNum);

                    // set texture pitch
                    texture.pitch = static_cast<uint16_t>(rowPitch);

                    // Upload only first mip
                    m_context->UpdateSubresource(texture.texture, 0, nullptr, memory, static_cast<UINT>(rowPitch), 0);
                }
                else
                {
                    // Upload all mips
                    for (auto mipId = 0; mipId < command->mipLevels; mipId++)
                    {
                        const auto width = command->width >> mipId;
                        const auto height = command->height >> mipId;

                        size_t size;
                        size_t rowPitch;
                        size_t rowNum;
                        DXGICalculateSurfaceInfo(width, height, dxgiFormat, &size, &rowPitch, &rowNum);

                        // Update mip
                        m_context->UpdateSubresource(texture.texture, mipId, nullptr, memory, static_cast<UINT>(rowPitch), 0);
                        
                        // Adjust memory pointer
                        memory += size;
                    }
                }
            }

            if (createDepthBuffer)
            {
                D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};

                depthStencilViewDesc.Format = DGXI_TextureFormats[command->textureFormat][2];
                depthStencilViewDesc.ViewDimension = m_msaaSampleCount > 1
                                                         ? D3D11_DSV_DIMENSION_TEXTURE2DMS
                                                         : D3D11_DSV_DIMENSION_TEXTURE2D;
                depthStencilViewDesc.Texture2D.MipSlice = 0;

                DX_CALL(m_device->CreateDepthStencilView(texture.texture, &depthStencilViewDesc, &texture.dsv));

                D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
                srvDesc.Format = DGXI_TextureFormats[command->textureFormat][1];
                srvDesc.ViewDimension = m_msaaSampleCount > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
                srvDesc.Texture2D.MipLevels = mipLevels;
                srvDesc.Texture2D.MostDetailedMip = 0;

                DX_CALL(m_device->CreateShaderResourceView(texture.texture, &srvDesc, &texture.srv));
            }
            else if(textureDesc.BindFlags != 0)
            {
                D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
                srvDesc.Format = DGXI_TextureFormats[command->textureFormat][1];
                srvDesc.ViewDimension = m_msaaSampleCount > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
                srvDesc.Texture2D.MipLevels = command->generateMips ? -1 : mipLevels;
                srvDesc.Texture2D.MostDetailedMip = 0;

                DX_CALL(m_device->CreateShaderResourceView(texture.texture, &srvDesc, &texture.srv));

                if(command->generateMips)
                {
                    m_context->GenerateMips(texture.srv);
                }

                if (command->renderTarget)
                {
                    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
                    rtvDesc.Format = DGXI_TextureFormats[command->textureFormat][1];
                    rtvDesc.ViewDimension = m_msaaSampleCount > 1 ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
                    rtvDesc.Texture2D.MipSlice = 0;

                    DX_CALL(m_device->CreateRenderTargetView(texture.texture, &rtvDesc, &texture.rtv));
                }
            }

            texture.width = command->width;
            texture.height = command->height;
            texture.format = command->textureFormat;
            texture.mips = mipLevels;
        }

        void WorkerThreadInstance::Execute_ApplyTexture2D(Command_ApplyTexture2D* command)
        {
            auto& texture = m_textures[command->handle.idx];
            ASSERT(texture.texture != nullptr);
            ASSERT(m_currentShader != nullptr);

            // bind the texture
            m_currentShader->BindResource(m_context, command->slot, texture.srv);
        }

        void WorkerThreadInstance::Execute_ApplyTextureArray2D(Command_ApplyTextureArray2D* command)
        {
            ASSERT(command->textureCount > 0);

            ID3D11ShaderResourceView* resources[32];
            for(auto i = 0u; i < command->textureCount; i ++)
            {
                auto& texture = m_textures[command->handles[i].idx];
                ASSERT(texture.texture != nullptr);
                ASSERT(m_currentShader != nullptr);

                resources[i] = texture.srv;
            }

            // bind the textures
            m_currentShader->BindResources(m_context, command->slot, resources, command->textureCount);
        }

        void WorkerThreadInstance::Execute_ResizeTexture2D(Command_ResizeTexture2D* command)
        {
            auto& texture = m_textures[command->handle.idx];
            ASSERT(texture.texture != nullptr);

            D3D11_TEXTURE2D_DESC textureDesc;
            texture.texture->GetDesc(&textureDesc);
            ADD_APICALL();

            // check if this texture has RTV, if so, this is a render target texture
            const auto isRenderTarget = texture.rtv != nullptr;

            // release the texture and all of it's views
            SafeRelease(texture.texture);
            SafeRelease(texture.srv);
            SafeRelease(texture.rtv);
            SafeRelease(texture.dsv);

            // rebuild texture based on the texture description
            Command_CreateTexture2D fakeCommand;

            fakeCommand.handle = command->handle;
            fakeCommand.mipLevels = textureDesc.MipLevels;
            fakeCommand.renderTarget = isRenderTarget;
            fakeCommand.width = command->width;
            fakeCommand.height = command->height;
            fakeCommand.textureFormat = texture.format;
            fakeCommand.dataSize = 0u;
            fakeCommand.memory = nullptr;
            fakeCommand.generateMips = texture.mips > 1;

            // create new texture
            Execute_CreateTexture2D(&fakeCommand);
        }

        void WorkerThreadInstance::Execute_UpdateTexture2D(Command_UpdateTexture2D* command)
        {
            RHIDirectX11::m_instance->UpdateTextureSubresource(command->handle, command->data, command->dataSize, command->subresourceId);
        }

        void WorkerThreadInstance::Execute_UpdateViewTexture2D(Command_UpdateViewTexture2D* command)
        {
            auto& texture = m_textures[command->handle.idx];
            ASSERT(texture.texture != nullptr);

            // Release previous srv
            SafeRelease(texture.srv);

            // TODO: Check if this texture view is bound... if so, unbind

            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            srvDesc.Format = DGXI_TextureFormats[texture.format][1];
            srvDesc.ViewDimension = m_msaaSampleCount > 1 ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = command->mipLevels;
            srvDesc.Texture2D.MostDetailedMip = command->mostDetailedMip;

            DX_CALL(m_device->CreateShaderResourceView(texture.texture, &srvDesc, &texture.srv));
        }

        void WorkerThreadInstance::Execute_DestroyTexture2D(Command_DestroyTexture2D* command)
        {
            auto& texture = m_textures[command->handle.idx];
            ASSERT(texture.texture != nullptr);

            // Destroy the texture
            texture.Dispose();
        }

        void WorkerThreadInstance::Execute_ApplyRenderTexture2D(Command_ApplyRenderTexture2D* command)
        {
            auto& texture = m_textures[command->handle.idx];
            ASSERT(texture.texture != nullptr);
            ASSERT(texture.rtv != nullptr);

            m_context->OMSetRenderTargets(1u, &texture.rtv, nullptr);
            ADD_APICALL();
        }

        void WorkerThreadInstance::Execute_ClearRenderTexture2D(Command_ClearRenderTexture2D* command)
        {
            auto& texture = m_textures[command->handle.idx];
            ASSERT(texture.rtv != nullptr);

            m_context->ClearRenderTargetView(texture.rtv, reinterpret_cast<float*>(&command->color));
            ADD_APICALL();
        }

#pragma endregion

#pragma region RHI base implementation
        void RHIDirectX11::kickFrameEvent()
        {
            for (auto thread : m_workerThreads)
            {
                if (!thread)
                    break;

                SetEvent(m_workerFrameEvents[thread->threadId]);
            }
        }

        void RHIDirectX11::waitForGPU()
        {
            // TODO: impl
        }

        void RHIDirectX11::PreFrameRender()
        {
            // reset previous states
            m_deviceContext->ClearState();

            // push stats
            m_lastApiCalls = m_apiCalls;
            m_lastDrawCalls = m_drawCalls;
            m_lastCommandCount = m_commandCount;
            m_lastIndicesDrawn = m_indicesDrawn;
            m_lastVerticesDrawn = m_verticesDrawn;

            // reset stats
            m_apiCalls = 0;
            m_drawCalls = 0;
            m_commandCount = 0;
            m_indicesDrawn = 0;
            m_verticesDrawn = 0;
        }

        void RHIDirectX11::assignCommands()
        {
            uint32_t dataBegin[RENDERER_MAX_RENDER_THREADS] = {};
            uint32_t dataEnd[RENDERER_MAX_RENDER_THREADS] = {};
            uint32_t commandCount[RENDERER_MAX_RENDER_THREADS] = {};

            // Assign new commands
            commandList.Assign(m_workerThreadCount, dataBegin, dataEnd, commandCount);

            // Write assigned commands info to all worker threads
            for (auto i = 0; i < m_workerThreadCount; i++)
            {
                const auto size = dataEnd[i] - dataBegin[i];

                // resize to the given size
                m_workerThreads[i]->m_commandList.Resize(size);
                m_workerThreads[i]->m_commandCount = commandCount[i];

                // Count the command count!
                m_commandCount += commandCount[i];

                // copy data
                memcpy(m_workerThreads[i]->m_commandList.cmdlist, static_cast<byte*>(commandList.cmdlist) + dataBegin[i], size);
            }
        }

        void RHIDirectX11::Initialize(Settings::_enum settings, RenderFlags::_enum flags)
        {
            m_instance = this;
            m_settings = settings;
            m_renderFlags = flags;
            m_defaultFlags = flags;
            m_running = true;

            if (m_renderFlags & RenderFlags::TripleBuffered)
            {
                ASSERT(false); // D3D11 does not support tripple buffering, yet!
                return;
            }

            const auto
#ifdef _DEBUG
            deviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
            deviceFlags = 0;
#endif

            D3D_FEATURE_LEVEL level;
            DX_CALL(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, nullptr, 0, D3D11_SDK_VERSION, &m_device, &level, &m_deviceContext));
            _ASSERT_(level >= D3D_FEATURE_LEVEL_11_0, "DirectX 11 is not supported on this hardware!");

#ifdef _DEBUG
            m_device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_debug));
#endif

            // Get CPU count
            auto cpuCount = static_cast<int>(std::thread::hardware_concurrency());
            if (m_settings & Settings::SingleThreaded)
                cpuCount = 1;

            // select multi-sampling count
            m_msaaSampleCount = 1;
            if (m_renderFlags & RenderFlags::MSAAx2)
                m_msaaSampleCount *= 2;
            if (m_renderFlags & RenderFlags::MSAAx4)
                m_msaaSampleCount *= 4;

            // Create depth stencil state
            D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

            depthStencilDesc.DepthEnable = true;
            depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

            depthStencilDesc.StencilEnable = false;
            depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
            depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

            depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
            depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

            depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
            depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

            // create regular DSS
            DX_CALL(m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState));

            // create no-depth DSS
            depthStencilDesc.DepthEnable = false;
            depthStencilDesc.StencilEnable = true;
            DX_CALL(m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState_NoDepth));

            // create no-stencil DSS
            depthStencilDesc.DepthEnable = true;
            depthStencilDesc.StencilEnable = false;
            DX_CALL(m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState_NoStencil));

            // create disabled DSS
            depthStencilDesc.DepthEnable = false;
            depthStencilDesc.StencilEnable = false;
            DX_CALL(m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState_Disabled));

            // create regular RS
            D3D11_RASTERIZER_DESC rasterizerDesc = {};
            rasterizerDesc.AntialiasedLineEnable = m_msaaSampleCount > 1;
            rasterizerDesc.MultisampleEnable = m_msaaSampleCount > 1;
            rasterizerDesc.CullMode = D3D11_CULL_BACK;
            rasterizerDesc.FillMode = D3D11_FILL_SOLID;
            rasterizerDesc.DepthClipEnable = true;

            DX_CALL(m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState));

            // create wireframe RC
            rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
            DX_CALL(m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState_WireFrame));

            // Create overlay blend-state
            D3D11_BLEND_DESC blendDesc = {};
            auto& blendTarget = blendDesc.RenderTarget[0] = {};

            blendDesc.AlphaToCoverageEnable = false;
            blendDesc.IndependentBlendEnable = false;

            blendTarget.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
            blendTarget.BlendEnable = true;
            blendTarget.BlendOp = D3D11_BLEND_OP_ADD;
            blendTarget.BlendOpAlpha = D3D11_BLEND_OP_ADD;
            blendTarget.SrcBlend = D3D11_BLEND_SRC_ALPHA;
            blendTarget.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
            blendTarget.SrcBlendAlpha = D3D11_BLEND_ONE;
            blendTarget.DestBlendAlpha = D3D11_BLEND_ONE;

            DX_CALL(m_device->CreateBlendState(&blendDesc, &m_blendState_Overlay));

            // Spawn Worker Threads
            for (auto i = 0; i < cpuCount && i < RENDERER_MAX_RENDER_THREADS; i++)
            {
                auto workerInstance = new WorkerThreadInstance;

                // Create frame done signal event
                m_workerFrameEvents[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
                ASSERT(m_workerFrameEvents[i] != nullptr);

                // Create worker finish signal event
                m_workerFinishEvents[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
                ASSERT(m_workerFinishEvents[i] != nullptr);

                workerInstance->threadId = i;
                workerInstance->m_commandList = {};
                workerInstance->m_commandList.Initialize();

                // Initialize worker
                workerInstance->InitializeWorker();

                // Setup thread when we are going to be multi threading
                if (!(m_settings & Settings::SingleThreaded))
                {
                    workerInstance->thread = std::thread([workerInstance] { workerInstance->WorkerThread(); });
                }

                m_workerThreads[i] = workerInstance;
                m_workerThreadCount++;
            }

            // Create command list
            commandList = {};
            commandList.Initialize();
        }

        void RHIDirectX11::Shutdown()
        {
            m_running = false;

            // Kick threads
            kickFrameEvent();

            // Wait for worker threads to finish
            for (auto&& thread : m_workerThreads)
            {
                if (thread)
                {
                    if (thread->thread.joinable())
                        thread->thread.join();

                    thread->Cleanup();
                    delete thread;
                }

                thread = nullptr;
            }

            // clear state
            m_deviceContext->ClearState();

            // Destroy outputs (if not released)
            for (auto i = 0; i < RENDERER_MAX_WINDOWS; i++)
            {
                SafeRelease(m_swapChains[i]);
            }

            // Destroy render buffers (if not released)
            for (auto i = 0; i < RENDERER_MAX_RENDER_BUFFERS; i++)
            {
                SafeRelease(m_renderBuffers[i]);
            }

            // Destroy shaders (if not released)
            for (auto i = 0; i < RENDERER_MAX_SHADER_PROGRAMS; i++)
            {
                SafeRelease(m_shaders[i]);
            }

            // Destroy vertex buffers (if not released)
            for (auto i = 0; i < RENDERER_MAX_VERTEX_BUFFERS; i++)
            {
                SafeRelease(m_vertexBuffers[i]);
            }

            // Destroy index buffers (if not released)
            for (auto i = 0; i < RENDERER_MAX_INDEX_BUFFERS; i++)
            {
                m_indexBuffers[i].Dispose();
            }

            // Destroy textures (if not released)
            for (auto i = 0; i < RENDERER_MAX_TEXTURES2D; i++)
            {
                m_textures[i].Dispose();
            }

            // Destroy command list
            commandList.Destroy();

            // Destroy D3D11 objects created on main-thread
            SafeRelease(m_depthStencilState);
            SafeRelease(m_rasterizerState);
            SafeRelease(m_deviceContext);
            SafeRelease(m_device);

#ifdef _DEBUG
            // Report leaked objects
            m_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
            SafeRelease(m_debug);
#endif
        }

        void RHIDirectX11::GetRenderStatistics(RenderStatistics* stats)
        {
            // collect stats
            stats->apiCallCount = m_lastApiCalls;
            stats->drawCallCount = m_lastDrawCalls;
            stats->commandCount = m_lastCommandCount;

            stats->verticesDrawn = m_lastVerticesDrawn;
            stats->indicesDrawn = m_lastIndicesDrawn;

            stats->flags = m_renderFlags;

            for (const auto& elem : m_vertexBuffers)
                stats->vertexBufferCount += elem != nullptr ? 1 : 0;

            for (const auto& elem : m_indexBuffers)
                stats->indexBufferCount += elem.buffer != nullptr ? 1 : 0;

            for (const auto& elem : m_textures)
                stats->texture2DCount += elem.texture != nullptr ? 1 : 0;

            for (const auto& elem : m_shaders)
                stats->shaderCount += elem != nullptr ? 1 : 0;

            for (const auto& elem : m_renderBuffers)
                stats->renderBufferCount += elem != nullptr ? 1 : 0;
        }

        void RHIDirectX11::GetContext(Renderer::RHIContext* context)
        {
            context->device = m_device;
            context->deviceContext = m_deviceContext;

            // copy windows data
            auto windowId = 0;
            for (auto& window : m_windows)
            {
                if (window.hwnd)
                {
                    auto& windowDesc = context->windows[windowId];

                    windowDesc.windowHandle = window.hwnd;
                    windowDesc.swapChain = window.swapChain;
                    windowDesc.backBuffer = m_renderBuffers[window.renderBuffer.idx]->GetRTVs()[0];
                }
                windowId++;
            }
        }

        void RHIDirectX11::Frame()
        {
            static auto FrameCount = 0;
            static auto FirstFrame = true;
            static std::vector<CComPtr<ID3D11CommandList>> commandLists = {};

            CPU_PROFILE_FUNCTION(0);

            if (FirstFrame)
            {
                FirstFrame = false;

                if (!(m_settings & Settings::SingleThreaded))
                {
                    // Signal that we have done our initialization
                    kickFrameEvent();
                }
            }

            Profiler::BeginProfile("SyncWorker");
            // Wait for all worker threads when Multi-Threaded
            if (!(m_settings & Settings::SingleThreaded))
                WaitForMultipleObjects(m_workerThreadCount, m_workerFinishEvents, true, INFINITE);
            Profiler::EndProfile();

            Profiler::BeginProfile("ExecCommandList");
            // Build command list array
            for (auto thread : m_workerThreads)
            {
                // Mannualy assing commands and process frame when Single-Threadeed
                if (m_settings & Settings::SingleThreaded)
                {
                    PreFrameRender();
                    assignCommands();
                    thread->ProcessFrame();
                }
                else
                {
                    // Finish D3D11 command list
                    CComPtr<ID3D11CommandList> commandList = nullptr;
                    DX_CALL(thread->m_context->FinishCommandList(FALSE, &commandList));

                    // Add current thread's command list
                    commandLists.push_back(commandList);

                    // Break now when Single-Threadeed
                    if (m_settings & Settings::SingleThreaded)
                        break;
                }
            }

            // Execute command lists
            for (auto&& commandList : commandLists)
            {
                m_deviceContext->ExecuteCommandList(commandList, TRUE);
            }
            Profiler::EndProfile();

            Profiler::BeginProfile("OnPresentBegin");
            // Call present begin event
            onPresentBegin.Invoke();
            Profiler::EndProfile();

            Profiler::BeginProfile("Present");
            // Present frame
            if (m_swapChain)
            {
                DXGI_PRESENT_PARAMETERS presentParams = {};
                m_swapChain->Present1(m_renderFlags & RenderFlags::VSync ? 1 : 0, 0, &presentParams);
            }
            Profiler::EndProfile();

            Profiler::BeginProfile("OnPresentEnd");
            // Call present end event
            onPresentEnd.Invoke();
            Profiler::EndProfile();

            Profiler::BeginProfile("StartNextFrame");
            if (!(m_settings & Settings::SingleThreaded))
            {
                PreFrameRender();

                // Assign new commands for all threads
                assignCommands();

                // Signal workers that this frame is now done
                kickFrameEvent();
            }

            // Clear command list array
            commandLists.clear();

            // Clear internal command list
            commandList.Clear();

            // Increment frame count
            FrameCount++;
            Profiler::EndProfile();
        }

        void RHIDirectX11::CreateVertexBuffer(VertexBufferHandle handle, uint vertexCount, uint vertexSize, bool dynamic, RendererMemory buffer)
        {
            const auto thread = m_workerThreads[0];

            Command_CreateVertexBuffer command = {};
            command.handle = handle;
            command.dynamic = dynamic;
            command.memory = buffer;
            command.vertexCount = vertexCount;
            command.vertexSize = vertexSize;
            
            thread->Execute_CreateVertexBuffer(&command);
        }

        void RHIDirectX11::CreateIndexBuffer(IndexBufferHandle handle, uint indexCount, bool is32bit, bool dynamic, RendererMemory buffer)
        {
            const auto thread = m_workerThreads[0];

            Command_CreateIndexBuffer command = {};
            command.handle = handle;
            command.dynamic = dynamic;
            command.memory = buffer;
            command.indexCount = indexCount;
            command.indexSize = is32bit ? 32 : 16;

            thread->Execute_CreateIndexBuffer(&command);
        }

        void RHIDirectX11::DestroyVertexBuffer(VertexBufferHandle handle)
        {
            const auto thread = m_workerThreads[0];

            Command_DestroyVertexBuffer command = {};
            command.handle = handle;
            thread->Execute_DestroyVertexBuffer(&command);
        }

        void RHIDirectX11::DestroyIndexBuffer(IndexBufferHandle handle)
        {
            const auto thread = m_workerThreads[0];

            Command_DestroyIndexBuffer command = {};
            command.handle = handle;
            thread->Execute_DestroyIndexBuffer(&command);
        }
#pragma endregion

#pragma region RHI interface implementation
        void RHIDirectX11::CreateWindowHandle(WindowHandle window, RenderBufferHandle renderBufferHandle,
                                              void* windowHandle)
        {
            waitForGPU();

            auto hWnd = static_cast<HWND>(windowHandle);

            RECT windowRect = {};
            GetClientRect(hWnd, &windowRect);

            UINT width = windowRect.right - windowRect.left;
            UINT height = windowRect.bottom - windowRect.top;

            IDXGIDevice1* device;
            DX_CALL(m_device->QueryInterface(__uuidof(IDXGIDevice1), reinterpret_cast<void**>(&device)));

            IDXGIAdapter1* adapter;
            DX_CALL(device->GetParent(__uuidof(IDXGIAdapter1), reinterpret_cast<void**>(&adapter)));

            IDXGIFactory1* factory;
            DX_CALL(adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&factory)));

            // Create the Swap Chain
            DXGI_SAMPLE_DESC sampleDesc;
            sampleDesc.Count = m_msaaSampleCount;
            sampleDesc.Quality = 0;

            DXGI_MODE_DESC backBufferDesc = {};
            backBufferDesc.Width = width; // buffer width
            backBufferDesc.Height = height; // buffer height
            backBufferDesc.Format = m_settings & Settings::BGRAFrameBuffer
                                        ? DXGI_FORMAT_B8G8R8A8_UNORM
                                        : DXGI_FORMAT_R8G8B8A8_UNORM;

            DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
            swapChainDesc.BufferDesc = backBufferDesc; // our back buffer description
            swapChainDesc.SampleDesc = sampleDesc; // our multi-sampling description
            swapChainDesc.BufferCount = 3; // number of buffers we have
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
            swapChainDesc.OutputWindow = hWnd; // handle to our window
            swapChainDesc.Windowed = true;

            IDXGISwapChain* tempSwapChain;
            DX_CALL(factory->CreateSwapChain(m_device, &swapChainDesc, &tempSwapChain));

            m_swapChains[window.idx] = static_cast<IDXGISwapChain1*>(tempSwapChain);

            // Apply the output as current
            m_swapChain = m_swapChains[window.idx];

            ID3D11RenderTargetView* renderTargets[FrameBufferCount] = {};
            
            ID3D11Resource* resource = nullptr;
            DX_CALL(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&resource)));

            m_device->CreateRenderTargetView(resource, nullptr, &renderTargets[0]);
            SafeRelease(resource);

            // WARNING: No depth stencil view for back buffers!
            // TODO: Create depth stencil view

            WindowDesc windowDesc;
            windowDesc.renderBuffer = renderBufferHandle;
            windowDesc.hwnd = hWnd;
            windowDesc.swapChain = m_swapChain;

            m_windows[window.idx] = windowDesc;

            // Set current window TODO: Multi-window support
            m_workerThreads[0]->m_currentWindow = &m_windows[window.idx];

            // Build 'proxy' render buffer
            m_renderBuffers[renderBufferHandle.idx] = RHIDirectX11_RenderBuffer::Create(
                m_device, width, height, FrameBufferCount, renderTargets, nullptr);

            SafeRelease(factory);
            SafeRelease(adapter);
            SafeRelease(device);
        }

        void RHIDirectX11::ResizeWindow(WindowHandle window, int width, int height)
        {
        }

        void RHIDirectX11::UpdateTextureSubresource(const Texture2DHandle textureHandle, void* data, size_t dataSize, uint8_t subresourceId)
        {
            const auto textureDesc = m_textures[textureHandle.idx];
            ASSERT(textureDesc.texture != nullptr);

            const auto dxgiFormat = DGXI_TextureFormats[textureDesc.format][0];

            // Calculate subresource pitch
            size_t size;
            size_t pitch;
            size_t rows;
            DXGICalculateSurfaceInfo(textureDesc.width >> subresourceId, textureDesc.height >> subresourceId, dxgiFormat, &size, &pitch, &rows);

            m_deviceContext->UpdateSubresource(textureDesc.texture, subresourceId, nullptr, data, static_cast<UINT>(pitch), static_cast<uint>(dataSize));
        }

        void RHIDirectX11::CopyTextureSubresource(Texture2DHandle targetTexture, void* sourceTexturePtr, uint8_t subresourceId)
        {
            const auto textureDesc = m_textures[targetTexture.idx];
            ASSERT(textureDesc.texture != nullptr);

            const auto sourceTexture = static_cast<ID3D11Texture2D*>(sourceTexturePtr);
            //m_deviceContext->CopyResource(textureDesc.texture, sourceTexture);
            m_deviceContext->CopySubresourceRegion(textureDesc.texture, subresourceId, 0u, 0u, 0u, sourceTexture, subresourceId, nullptr);
        }

        void RHIDirectX11::GetTextureSubresource(Texture2DHandle textureHandle, void* buffer, size_t bufferSize, uint8_t subresourceId)
        {
            const auto textureDesc = m_textures[textureHandle.idx];
            ASSERT(textureDesc.texture != nullptr);

            auto pitch = 0;
            if (textureDesc.pitch > 0)
            {
                pitch = textureDesc.pitch;
            }
            else
            {
                const auto formatSize = DXGIFormatGetSize(DGXI_TextureFormats[textureDesc.format][0]);
                pitch = textureDesc.width * formatSize / 8;
            }
            const auto textureSize = textureDesc.height * pitch;

            ASSERT(textureSize == bufferSize);

            const auto texture = textureDesc.texture;

            D3D11_MAPPED_SUBRESOURCE subresource;
            m_deviceContext->Map(texture, 0, D3D11_MAP_READ, 0, &subresource);

            memcpy(buffer, subresource.pData, bufferSize);

            m_deviceContext->Unmap(texture, 0);
        }
#pragma endregion
    }
}

#endif
