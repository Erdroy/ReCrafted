// ReCrafted (c) 2016-2018 Always Too Late

#include "DirectX11.h"

#include "RHIDirectX11.h"
#include "Platform/Platform.h"

#if RENDERER_RHI_D3D11

#include "../../Renderer.hpp"
#include "../../RendererConfig.h"

#include "../RHIContext.h"

#include "Internal/RHIDirectX11_RenderBuffer.h"
#include "Internal/RHIDirectX11_Shader.h"

#include <json.hpp>
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

        const int FrameBufferCount = 3;

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
        IDXGISwapChain* m_swapChains[RENDERER_MAX_WINDOWS] = {};
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
        ID3D11Device* m_device = nullptr;
        ID3D11DeviceContext* m_deviceContext = nullptr;


        // == pointer-only resources (DO NOT RELEASE!) ==
        IDXGISwapChain* m_swapChain = nullptr;

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
            FORCEINLINE void Execute_ResizeTexture2D(Command_ResizeTexture2D* command);
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
            Platform::SetThreadName("Render Thread");

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
        }

        void WorkerThreadInstance::ResetFlags()
        {
            m_renderFlags = m_defaultFlags;

            // Setup default states (reset)
            for (var i = 0u; 1u << i < RenderFlags::Count; i++)
            {
                cvar flag = RenderFlags::_enum(1u << i);  // NOLINT
                cvar flagState = GetFlag(flag);

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
            var position = 0u;
            for (var i = 0u; i < m_commandCount; i++)
            {
                var header = m_commandList.Read_CommandHeader(&position);
                position -= sizeof header;

                ExecuteCommand(header, &position);
            }
        }

        void WorkerThreadInstance::ExecuteCommand(CommandHeader::_enum header, uint32_t* position)
        {
#define DEFINE_COMMAND_EXECUTOR(name) \
            case CommandHeader::name: { \
                var command = m_commandList.ReadCommand<Command_##name>(position);\
                Execute_##name(&command);\
                break; \
		    }

            switch (header)
            {
            case CommandHeader::ApplyWindow:
                {
                    cvar command = m_commandList.ReadCommand<Command_ApplyWindow>(position);
                    cvar idx = command.window.idx;

                    // set window's swapchain as current
                    m_swapChain = m_swapChains[idx];
                    break;
                }
            case CommandHeader::DestroyWindow:
                {
                    cvar command = m_commandList.ReadCommand<Command_DestroyWindow>(position);
                    cvar idx = command.window.idx;

                    SafeRelease(m_swapChains[idx]);

                    var windowDesc = GetWindowDescription(command.window);

                    var renderBufferIdx = windowDesc.renderBuffer.idx;
                    SafeRelease(m_renderBuffers[renderBufferIdx]);

                    break;
                }
            case CommandHeader::SetShaderValue:
                {
                    float data[16];
                    cvar command = m_commandList.ReadCommand<Command_SetShaderValue>(position);
                    m_commandList.Read(data, position, command.dataSize);

                    rvar shader = m_shaders[command.shader.idx];
                    shader->SetValue(command.bufferId, command.fieldId, data, command.dataSize);
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
            DEFINE_COMMAND_EXECUTOR(ResizeTexture2D);
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
            cvar outputTexture = m_textures[command->targetTexture.idx].texture;
            ASSERT(outputTexture != nullptr);

            cvar renderBuffer = m_renderBuffers[m_currentWindow->renderBuffer.idx];
            cvar renderTarget = renderBuffer->GetRTVs()[0];

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
                cvar stencilEnabled = m_renderFlags & RenderFlags::DepthStencil;
                cvar depthEnabled = m_renderFlags & RenderFlags::DepthTest;

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
            rvar renderBuffer = m_renderBuffers[command->handle.idx];
            cvar swapChain = m_swapChains[command->windowHandle.idx];

            ASSERT(renderBuffer != nullptr);
            ASSERT(swapChain != nullptr);

            cvar bufferCount = m_renderFlags & RenderFlags::TripleBuffered ? FrameBufferCount : 1;
            cvar bufferFormat = m_settings & Settings::BGRAFrameBuffer
                                    ? DXGI_FORMAT_B8G8R8A8_UNORM
                                    : DXGI_FORMAT_R8G8B8A8_UNORM;

            // Destroy render targets
            cvar rtvs = renderBuffer->GetRTVs();
            for (var i = 0; i < bufferCount; i ++)
            {
                SafeRelease(rtvs[i]);
            }

            // Destroy depth buffer
            if (renderBuffer->GetDSV())
                renderBuffer->GetDSV()->Release();

            DX_CALL(swapChain->ResizeBuffers(static_cast<uint>(bufferCount), command->width, command->height, bufferFormat, 0u));

            ID3D11RenderTargetView* renderTargets[FrameBufferCount] = {};
            for (var i = 0; i < bufferCount; i++)
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
            cvar shaderIdx = command->shader.idx;

            std::ifstream input(command->fileName);
            std::string content((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));

            var jsonData = json::parse(content);
            m_shaders[shaderIdx] = RHIDirectX11_Shader::Create(m_device, jsonData);

        }
        
        void WorkerThreadInstance::Execute_ApplyShader(Command_ApplyShader* command)
        {
            var shaderIdx = command->shader.idx;
            var shader = m_shaders[shaderIdx];

            shader->Bind(m_context, command->passId);

            m_currentShader = shader;
        }

        void WorkerThreadInstance::Execute_DestroyShader(Command_DestroyShader* command)
        {
            var shaderIdx = command->shader.idx;
            SafeRelease(m_shaders[shaderIdx]);
        }

        void WorkerThreadInstance::Execute_CreateVertexBuffer(Command_CreateVertexBuffer* command)
        {
            rvar buffer = m_vertexBuffers[command->handle.idx];
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
            rvar buffer = m_vertexBuffers[command->handle.idx];
            ASSERT(buffer != nullptr);

            ID3D11Buffer* buffers[] = {buffer};

            uint offset = 0u;
            uint stride = m_currentShader->GetStride();
            m_context->IASetVertexBuffers(0, 1, buffers, &stride, &offset);
            ADD_APICALL();
        }

        void WorkerThreadInstance::Execute_UpdateVertexBuffer(Command_UpdateVertexBuffer* command)
        {
            rvar buffer = m_vertexBuffers[command->handle.idx];
            ASSERT(buffer != nullptr);

            D3D11_MAPPED_SUBRESOURCE res = {};
            DX_CALL(m_context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res));

            cvar ptr = static_cast<byte*>(res.pData) + command->memoryOffset;
            memcpy(ptr, command->memory, command->memorySize);

            m_context->Unmap(buffer, 0);
            ADD_APICALL();
        }

        void WorkerThreadInstance::Execute_DestroyVertexBuffer(Command_DestroyVertexBuffer* command)
        {
            rvar buffer = m_vertexBuffers[command->handle.idx];
            ASSERT(buffer != nullptr);

            SafeRelease(buffer);

            (*RHIDirectX11::m_instance->freeVertexBuffer)(command->handle);
        }

        void WorkerThreadInstance::Execute_CreateIndexBuffer(Command_CreateIndexBuffer* command)
        {
            rvar buffer = m_indexBuffers[command->handle.idx];
            ASSERT(buffer.buffer == nullptr);

            buffer.is32bit = command->indexSize == 32;

            cvar bufferSizeBytes = buffer.is32bit ? 4 : 2;

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
            rvar buffer = m_indexBuffers[command->handle.idx];
            ASSERT(buffer.buffer != nullptr);

            m_context->IASetIndexBuffer(buffer.buffer, buffer.is32bit ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT,
                                        0u);
            ADD_APICALL();
        }

        void WorkerThreadInstance::Execute_UpdateIndexBuffer(Command_UpdateIndexBuffer* command)
        {
            rvar buffer = m_indexBuffers[command->handle.idx];
            ASSERT(buffer.buffer != nullptr);

            D3D11_MAPPED_SUBRESOURCE res;
            DX_CALL(m_context->Map(buffer.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res));

            cvar ptr = static_cast<byte*>(res.pData) + command->memoryOffset;
            memcpy(ptr, command->memory, command->memorySize);

            m_context->Unmap(buffer.buffer, 0);
            ADD_APICALL();
        }

        void WorkerThreadInstance::Execute_DestroyIndexBuffer(Command_DestroyIndexBuffer* command)
        {
            rvar buffer = m_indexBuffers[command->handle.idx];
            ASSERT(buffer.buffer != nullptr);

            buffer.Dispose();

            (*RHIDirectX11::m_instance->freeIndexBuffer)(command->handle);
        }

        void WorkerThreadInstance::Execute_CreateRenderBuffer(Command_CreateRenderBuffer* command)
        {
            rvar buffer = m_renderBuffers[command->handle.idx];
            ASSERT(buffer == nullptr);

            ASSERT(command->texturesCount > 0);
            ASSERT(command->width >= 16);
            ASSERT(command->height >= 16);

            // select rtv's
            std::vector<ID3D11RenderTargetView*> rtvs = {};
            for (var i = 0; i < command->texturesCount; i ++)
            {
                cvar texture = m_textures[command->renderTargets[i].idx];
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
            rvar buffer = m_renderBuffers[command->handle.idx];
            ASSERT(buffer != nullptr);

            ASSERT(command->width >= 16);
            ASSERT(command->height >= 16);

            // set size
            buffer->SetSize(command->width, command->height);

            // rebind all textures
            // select rtv's
            std::vector<ID3D11RenderTargetView*> rtvs = {};
            for (var i = 0u; i < command->texturesCount; i++)
            {
                cvar texture = m_textures[command->renderTargets[i].idx];
                ASSERT(texture.rtv != nullptr);
                rtvs.push_back(texture.rtv);
            }
            buffer->SetRTVs(rtvs.data());

            if (RENDERER_CHECK_HANDLE(command->depthTarget))
                buffer->SetDSV(m_textures[command->depthTarget.idx].dsv);
        }

        void WorkerThreadInstance::Execute_ClearRenderBuffer(Command_ClearRenderBuffer* command)
        {
            rvar buffer = m_renderBuffers[command->handle.idx];
            ASSERT(buffer != nullptr);

            // TODO: check if we can pass frame index (is it back buffer - render buffer)

            buffer->Clear(m_context, command->color, command->depth);
        }

        void WorkerThreadInstance::Execute_ApplyRenderBuffer(Command_ApplyRenderBuffer* command)
        {
            rvar buffer = m_renderBuffers[command->handle.idx];
            ASSERT(buffer != nullptr);

            // TODO: check if we can pass frame index (is it back buffer - render buffer)

            buffer->Bind(m_context);

            m_currentRenderBuffer = buffer;
        }

        void WorkerThreadInstance::Execute_DestroyRenderBuffer(Command_DestroyRenderBuffer* command)
        {
            rvar buffer = m_renderBuffers[command->handle.idx];
            ASSERT(buffer != nullptr);

            SafeRelease(buffer);
        }

        void WorkerThreadInstance::Execute_CreateTexture2D(Command_CreateTexture2D* command)
        {
            rvar texture = m_textures[command->handle.idx];
            ASSERT(texture.texture == nullptr);

            cvar mipLevels = command->mipLevels == 0 ? 1 : command->mipLevels;
            cvar createDepthBuffer = command->textureFormat >= TextureFormat::D16;

            cvar rt = command->renderTarget || createDepthBuffer;

            D3D11_TEXTURE2D_DESC textureDesc;
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
                cvar rowPitch = command->width * (TextureFormatInfo[command->textureFormat][0] / 8);
                m_context->UpdateSubresource(texture.texture, 0, nullptr, command->memory, rowPitch, 0);
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
        }

        void WorkerThreadInstance::Execute_ApplyTexture2D(Command_ApplyTexture2D* command)
        {
            rvar texture = m_textures[command->handle.idx];
            ASSERT(texture.texture != nullptr);
            ASSERT(m_currentShader != nullptr);

            // bind the texture
            m_currentShader->BindResource(m_context, command->slot, texture.srv);
        }

        void WorkerThreadInstance::Execute_ResizeTexture2D(Command_ResizeTexture2D* command)
        {
            rvar texture = m_textures[command->handle.idx];
            ASSERT(texture.texture != nullptr);

            D3D11_TEXTURE2D_DESC textureDesc;
            texture.texture->GetDesc(&textureDesc);
            ADD_APICALL();

            // check if this texture has RTV, if so, this is a render target texture
            cvar isRenderTarget = texture.rtv != nullptr;

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

            // create new texture
            Execute_CreateTexture2D(&fakeCommand);
        }

        void WorkerThreadInstance::Execute_DestroyTexture2D(Command_DestroyTexture2D* command)
        {
            rvar texture = m_textures[command->handle.idx];
            ASSERT(texture.texture != nullptr);

            SafeRelease(texture.texture);
        }

        void WorkerThreadInstance::Execute_ApplyRenderTexture2D(Command_ApplyRenderTexture2D* command)
        {
            rvar texture = m_textures[command->handle.idx];
            ASSERT(texture.texture != nullptr);
            ASSERT(texture.rtv != nullptr);

            m_context->OMSetRenderTargets(1u, &texture.rtv, nullptr);
            ADD_APICALL();
        }

        void WorkerThreadInstance::Execute_ClearRenderTexture2D(Command_ClearRenderTexture2D* command)
        {
            rvar texture = m_textures[command->handle.idx];
            ASSERT(texture.rtv != nullptr);

            m_context->ClearRenderTargetView(texture.rtv, reinterpret_cast<float*>(&command->color));
            ADD_APICALL();
        }

#pragma endregion

#pragma region RHI base implementation
        void RHIDirectX11::kickFrameEvent()
        {
            for (var thread : m_workerThreads)
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
            for (var i = 0; i < m_workerThreadCount; i++)
            {
                cvar size = dataEnd[i] - dataBegin[i];

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

            D3D_FEATURE_LEVEL level;
            D3D_FEATURE_LEVEL featureLevels[] =
            {
                D3D_FEATURE_LEVEL_11_0,
                D3D_FEATURE_LEVEL_11_1
            };

            cvar
#ifdef _DEBUG
            deviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
            deviceFlags = 0;
#endif

            DX_CALL(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, featureLevels, 1, D3D11_SDK_VERSION, &m_device, &level, &m_deviceContext));

            // Get CPU count
            int cpuCount = std::thread::hardware_concurrency();
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
            rvar blendTarget = blendDesc.RenderTarget[0] = {};

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
            for (var i = 0; i < cpuCount && i < RENDERER_MAX_RENDER_THREADS; i++)
            {
                var workerInstance = new WorkerThreadInstance;

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
            var idx = 0;
            for (var thread : m_workerThreads)
            {
                if (thread)
                {
                    if (thread->thread.joinable())
                        thread->thread.join();

                    thread->Cleanup();
                    delete thread;
                }

                m_workerThreads[idx] = nullptr;
                idx++;
            }

            // clear state
            m_deviceContext->ClearState();

            // Destroy outputs (if not released)
            for (var i = 0; i < RENDERER_MAX_WINDOWS; i++)
            {
                SafeRelease(m_swapChains[i]);
            }

            // Destroy render buffers (if not released)
            for (var i = 0; i < RENDERER_MAX_RENDER_BUFFERS; i++)
            {
                SafeRelease(m_renderBuffers[i]);
            }

            // Destroy shaders (if not released)
            for (var i = 0; i < RENDERER_MAX_SHADER_PROGRAMS; i++)
            {
                SafeRelease(m_shaders[i]);
            }

            // Destroy vertex buffers (if not released)
            for (var i = 0; i < RENDERER_MAX_VERTEX_BUFFERS; i++)
            {
                SafeRelease(m_vertexBuffers[i]);
            }

            // Destroy index buffers (if not released)
            for (var i = 0; i < RENDERER_MAX_INDEX_BUFFERS; i++)
            {
                m_indexBuffers[i].Dispose();
            }

            // Destroy textures (if not released)
            for (var i = 0; i < RENDERER_MAX_TEXTURES2D; i++)
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

            for (crvar elem : m_vertexBuffers)
                stats->vertexBufferCount += elem != nullptr ? 1 : 0;

            for (crvar elem : m_indexBuffers)
                stats->indexBufferCount += elem.buffer != nullptr ? 1 : 0;

            for (crvar elem : m_textures)
                stats->texture2DCount += elem.texture != nullptr ? 1 : 0;

            for (crvar elem : m_shaders)
                stats->shaderCount += elem != nullptr ? 1 : 0;

            for (crvar elem : m_renderBuffers)
                stats->renderBufferCount += elem != nullptr ? 1 : 0;
        }

        void RHIDirectX11::GetContext(Renderer::RHIContext* context)
        {
            context->device = m_device;
            context->deviceContext = m_deviceContext;

            // copy windows data
            var windowId = 0;
            for (rvar window : m_windows)
            {
                if (window.hwnd)
                {
                    rvar windowDesc = context->windows[windowId];

                    windowDesc.windowHandle = window.hwnd;
                    windowDesc.swapChain = window.swapChain;
                    windowDesc.backBuffer = m_renderBuffers[window.renderBuffer.idx]->GetRTVs()[0];
                }
                windowId++;
            }
        }

        void RHIDirectX11::Frame()
        {
            static var FrameCount = 0;
            static var FirstFrame = true;
            static std::vector<CComPtr<ID3D11CommandList>> commandLists = {};

            if (FirstFrame)
            {
                FirstFrame = false;

                if (!(m_settings & Settings::SingleThreaded))
                {
                    // Signal that we have done our initialization
                    kickFrameEvent();
                }
            }

            // Wait for all worker threads when Multi-Threaded
            if (!(m_settings & Settings::SingleThreaded))
                WaitForMultipleObjects(m_workerThreadCount, m_workerFinishEvents, true, INFINITE);

            // Build command list array
            for (var thread : m_workerThreads)
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
            for (var&& commandList : commandLists)
            {
                m_deviceContext->ExecuteCommandList(commandList, TRUE);
            }

            // Call beforePresent event
            callbacksBeforeRender.Invoke();

            // Present frame
            if (m_swapChain)
            {
                DX_CALL(m_swapChain->Present(m_renderFlags & RenderFlags::VSync ? 1 : 0, 0));
            }

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
        }

        void RHIDirectX11::CreateVertexBuffer(VertexBufferHandle handle, uint vertexCount, uint vertexSize, bool dynamic, RendererMemory buffer)
        {
            cvar thread = m_workerThreads[0];

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
            cvar thread = m_workerThreads[0];

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
            cvar thread = m_workerThreads[0];

            Command_DestroyVertexBuffer command = {};
            command.handle = handle;
            thread->Execute_DestroyVertexBuffer(&command);
        }

        void RHIDirectX11::DestroyIndexBuffer(IndexBufferHandle handle)
        {
            cvar thread = m_workerThreads[0];

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

            var hWnd = static_cast<HWND>(windowHandle);

            RECT windowRect = {};
            GetClientRect(hWnd, &windowRect);

            UINT width = windowRect.right - windowRect.left;
            UINT height = windowRect.bottom - windowRect.top;

            var bufferCount = m_renderFlags & RenderFlags::TripleBuffered ? FrameBufferCount : 1;

            IDXGIDevice* device;
            DX_CALL(m_device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&device)));

            IDXGIAdapter* adapter;
            DX_CALL(device->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&adapter)));

            IDXGIFactory* factory;
            DX_CALL(adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory)));

            // Create the Swap Chain
            DXGI_SAMPLE_DESC sampleDesc;
            sampleDesc.Count = m_msaaSampleCount;
            sampleDesc.Quality = 0;

            DXGI_MODE_DESC backBufferDesc = {}; // this is to describe our display mode
            backBufferDesc.Width = width; // buffer width
            backBufferDesc.Height = height; // buffer height
            backBufferDesc.Format = m_settings & Settings::BGRAFrameBuffer
                                        ? DXGI_FORMAT_B8G8R8A8_UNORM
                                        : DXGI_FORMAT_R8G8B8A8_UNORM;

            // Describe and create the swap chain.
            DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
            swapChainDesc.BufferCount = bufferCount; // number of buffers we have
            swapChainDesc.BufferDesc = backBufferDesc; // our back buffer description
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            // this says the pipeline will render to this swap chain
            swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
            // dxgi will discard the buffer (data) after we call present
            swapChainDesc.OutputWindow = hWnd; // handle to our window
            swapChainDesc.SampleDesc = sampleDesc; // our multi-sampling description
            swapChainDesc.Windowed = true;
            // set to true, then if in fullscreen must call SetFullScreenState with true for full screen to get uncapped fps

            IDXGISwapChain* tempSwapChain;
            DX_CALL(factory->CreateSwapChain(m_device, &swapChainDesc, &tempSwapChain));

            m_swapChains[window.idx] = static_cast<IDXGISwapChain*>(tempSwapChain);

            // Apply the output as current
            m_swapChain = m_swapChains[window.idx];

            ID3D11RenderTargetView* renderTargets[FrameBufferCount] = {};
            for (var i = 0; i < bufferCount; i++)
            {
                ID3D11Resource* resource = nullptr;
                DX_CALL(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&resource)));

                m_device->CreateRenderTargetView(resource, nullptr, &renderTargets[i]);
                SafeRelease(resource);
            }

            // WARNING: No depth stencil view for back buffers!
            // TODO: Create depth stencil view

            WindowDesc windowDesc = {};
            windowDesc.renderBuffer = renderBufferHandle;
            windowDesc.hwnd = hWnd;
            windowDesc.swapChain = m_swapChain;

            m_windows[window.idx] = windowDesc;

            // Set current window TODO: Multi-window support
            m_workerThreads[0]->m_currentWindow = &m_windows[window.idx];

            // Build 'proxy' render buffer
            m_renderBuffers[renderBufferHandle.idx] = RHIDirectX11_RenderBuffer::Create(
                m_device, width, height, bufferCount, renderTargets, nullptr);

            SafeRelease(factory);
            SafeRelease(adapter);
            SafeRelease(device);
        }

        void RHIDirectX11::ResizeWindow(WindowHandle window, int width, int height)
        {
        }

        void RHIDirectX11::GetTextureData(Texture2DHandle textureHandle, void* buffer, size_t bufferSize)
        {
            cvar textureDesc = m_textures[textureHandle.idx];
            ASSERT(textureDesc.texture != nullptr);
            
            cvar formatSize = DXGIFormatGetSize(DGXI_TextureFormats[textureDesc.format][0]);
            cvar textureSize = textureDesc.width * textureDesc.height * formatSize;

            ASSERT(textureSize == bufferSize);

            cvar texture = textureDesc.texture;

            D3D11_MAPPED_SUBRESOURCE subresource;
            m_deviceContext->Map(texture, 0, D3D11_MAP_READ, 0, &subresource);

            memcpy(buffer, subresource.pData, bufferSize);

            m_deviceContext->Unmap(texture, 0);
        }
#pragma endregion
    }
}

#endif
