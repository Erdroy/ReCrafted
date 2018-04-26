// ReCrafted (c) 2016-2018 Always Too Late

#include "DirectX11.h"

#include "RHIDirectX11.h"
#include "Platform/Platform.h"
#include "Core/Memory.h"

#if RENDERER_RENDERER_D3D11

#include "../../Renderer.hpp"
#include "../../RendererConfig.h"

#include "Internal/RHIDirectX11_RenderBuffer.h"
#include "Internal/RHIDirectX11_Shader.h"

#include <json.hpp>
#include <thread>
#include <vector>

namespace Renderer
{
    using json = nlohmann::json;

    namespace RHI
    {
        class WorkerThreadInstance;

        const int FrameBufferCount = 3;


        // == common ==
        volatile bool               m_running;
        int                         m_workerThreadCount;
        Settings::_enum             m_settings;
        ResetFlags::_enum           m_resetFlags;


        // == events ==
        HANDLE                      m_workerFinishEvents[RENDERER_MAX_WORKER_THREADS];
        HANDLE                      m_workerFrameEvents[RENDERER_MAX_WORKER_THREADS];


        // == worker-related stuff ==
        WorkerThreadInstance*	    m_workerThreads[RENDERER_MAX_WORKER_THREADS] = {};


        // == resources ==
        IDXGISwapChain*		        m_swapChains[RENDERER_MAX_WINDOWS] = {};
        RHIDirectX11_Shader*        m_shaders[RENDERER_MAX_SHADER_PROGRAMS] = {};
        RHIDirectX11_RenderBuffer*  m_renderBuffers[RENDERER_MAX_RENDER_BUFFERS] = {};
        //RHIDirectX11_Texture2D*   m_textures2d[RENDERER_MAX_TEXTURES2D] = {};
        ID3D11Buffer*               m_vertexBuffers[RENDERER_MAX_VERTEX_BUFFERS] = {};
        //ID3D11Buffer*               m_indexBuffers[RENDERER_MAX_INDEX_BUFFERS] = {};


        // == d3d11 resources ==
        ID3D11Device*               m_device = nullptr;
        ID3D11DeviceContext*        m_deviceContext = nullptr;


        // == pointer-only resources (DO NOT RELEASE!) ==
        IDXGISwapChain*            m_swapChain = nullptr;

#pragma region WorkerThread impl
        class WorkerThreadInstance
        {
            friend class RHIDirectX11;

        private:
            HANDLE m_fenceEvent = {};

            uint32_t m_commandDataStart = 0u;
            uint32_t m_commandCount = 0u;

            ID3D11DeviceContext* m_context = nullptr;

        public:
            std::thread thread;

            int threadId;
            int frameCount;

            CommandList* commandList;

        public:
            void WaitForPreviousFrame();
            void WaitForExit();

            void InitializeWorker();
            void WorkerThread();

            void ProcessFrame();
            void ExecuteCommandList();
            void ExecuteCommand(CommandHeader::_enum header, uint32_t* position);

            void Cleanup();

        public:
            FORCEINLINE void Execute_ClearRenderBuffer(Command_ClearRenderBuffer* command) const;
            FORCEINLINE void Execute_ApplyRenderBuffer(Command_ApplyRenderBuffer* command) const;
            FORCEINLINE void Execute_CreateShader(Command_CreateShader* command) const;
            FORCEINLINE void Execute_ApplyShader(Command_ApplyShader* command) const;
            FORCEINLINE void Execute_DestroyShader(Command_DestroyShader* command) const;
            FORCEINLINE void Execute_CreateVertexBuffer(Command_CreateVertexBuffer* command) const;
            FORCEINLINE void Execute_ApplyVertexBuffer(Command_ApplyVertexBuffer* command) const;
            FORCEINLINE void Execute_DestroyVertexBuffer(Command_DestroyVertexBuffer* command) const;
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
            HRESULT hr;
            hr = m_device->CreateDeferredContext(0, &m_context);

            if(FAILED(hr))
            {
                Internal::Fatal("Failed to create D3D11 deferred context!");
                return;
            }

            m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

            if (m_fenceEvent == nullptr)
            {
                Internal::Fatal("Failed to create event!");
            }

            // we are done with worker thread setup now
        }

        void WorkerThreadInstance::WorkerThread()
        {
            var threadMemory = RPMallocThread();

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

            Internal::Log("Worker Thread stopped");
        }

        void WorkerThreadInstance::ProcessFrame()
        {
            WaitForPreviousFrame();
            ExecuteCommandList();
        }

        void WorkerThreadInstance::ExecuteCommandList()
        {
            if (m_commandCount == 0u)
                return;

            var position = m_commandDataStart;

            // Execute command list
            for (var i = 0u; i < m_commandCount; i++)
            {
                var header = commandList->Read_CommandHeader(&position);
                position -= sizeof header;

                ExecuteCommand(header, &position);
            }
        }

        void WorkerThreadInstance::ExecuteCommand(CommandHeader::_enum header, uint32_t* position)
        {
#define DEFINE_COMMAND_EXECUTOR(name) \
            case CommandHeader::name: { \
                var command = commandList->ReadCommand<Command_##name>(position);\
                Execute_##name(&command);\
                break; \
		    }

            switch (header)
            {
            case CommandHeader::Empty:
                break;
            case CommandHeader::ApplyWindow:
            {
                var command = commandList->ReadCommand<Command_ApplyWindow>(position);
                m_swapChain = m_swapChains[command.window.idx];
                break;
            }
            case CommandHeader::DestroyWindow:
            {
                var command = commandList->ReadCommand<Command_DestroyWindow>(position);

                var idx = command.window.idx;

                SafeRelease(m_swapChains[idx]);

                var renderBufferIdx = command.window.renderBuffer.idx;
                //SafeRelease(m_renderBuffers[renderBufferIdx]);

                break;
            }
            DEFINE_COMMAND_EXECUTOR(ClearRenderBuffer);
            DEFINE_COMMAND_EXECUTOR(ApplyRenderBuffer);
            DEFINE_COMMAND_EXECUTOR(CreateShader);
            DEFINE_COMMAND_EXECUTOR(ApplyShader);
            DEFINE_COMMAND_EXECUTOR(DestroyShader);
            DEFINE_COMMAND_EXECUTOR(CreateVertexBuffer);
            DEFINE_COMMAND_EXECUTOR(ApplyVertexBuffer);
            DEFINE_COMMAND_EXECUTOR(DestroyVertexBuffer);
            default: break;
            }
        }

        void WorkerThreadInstance::Cleanup()
        {
            SafeRelease(m_context);
        }
#pragma endregion

        // ========== WorkerThreadInstance::Executors ==========
#pragma region WorkerThread command execution implementation
        void WorkerThreadInstance::Execute_CreateShader(Command_CreateShader* command) const
        {
            cvar shaderIdx = command->shader.idx;

            File file;
            Platform::openFile(&file, command->fileName, OpenMode::OpenRead);

            const var data = new byte[file.FileSize];
            memset(data, 0, file.FileSize);
            file.read(data, file.FileSize);

            var jsonData = json::parse(data);
            m_shaders[shaderIdx] = RHIDirectX11_Shader::Create(m_device, jsonData);

            delete[] data;
            file.close();
        }

        void WorkerThreadInstance::Execute_ApplyShader(Command_ApplyShader* command) const
        {
            var shaderIdx = command->shader.idx;
            var shader = m_shaders[shaderIdx];
            shader->Bind(m_context, command->passId);
        }

        void WorkerThreadInstance::Execute_DestroyShader(Command_DestroyShader* command) const
        {
            var shaderIdx = command->shader.idx;
            SafeRelease(m_shaders[shaderIdx]);
        }

        void WorkerThreadInstance::Execute_CreateVertexBuffer(Command_CreateVertexBuffer* command) const
        {
            rvar buffer = m_vertexBuffers[command->handle.idx];
            _ASSERT(buffer == nullptr);

            // create buffer description
            D3D11_BUFFER_DESC desc = {};
            desc.Usage = command->dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
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
            var hr = m_device->CreateBuffer(&desc, command->memory ? &subresource_data : nullptr, &buffer);
            _ASSERT(SUCCEEDED(hr));
        }

        void WorkerThreadInstance::Execute_ApplyVertexBuffer(Command_ApplyVertexBuffer* command) const
        {
            rvar buffer = m_vertexBuffers[command->handle.idx];
            _ASSERT(buffer != nullptr);

            ID3D11Buffer* buffers[] = { buffer };

            D3D11_BUFFER_DESC desc = {};
            buffer->GetDesc(&desc);

            uint offset = 0u;
            m_deviceContext->IASetVertexBuffers(0, 1, buffers, &desc.StructureByteStride, &offset);
        }

        void WorkerThreadInstance::Execute_DestroyVertexBuffer(Command_DestroyVertexBuffer* command) const
        {
            rvar buffer = m_vertexBuffers[command->handle.idx];
            _ASSERT(buffer != nullptr);

            SafeRelease(buffer);
        }

        void WorkerThreadInstance::Execute_ClearRenderBuffer(Command_ClearRenderBuffer* command) const
        {
            var renderBufferIdx = command->renderBuffer.idx;

            // TODO: check if we can pass frame index (is it back buffer - render buffer)

            m_renderBuffers[renderBufferIdx]->Clear(m_context, command->color, 0x0, 0);
        }

        void WorkerThreadInstance::Execute_ApplyRenderBuffer(Command_ApplyRenderBuffer* command) const
        {
            var renderBufferIdx = command->renderBuffer.idx;

            // TODO: check if we can pass frame index (is it back buffer - render buffer)

            m_renderBuffers[renderBufferIdx]->Bind(m_context, 0);
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

        void RHIDirectX11::assignCommands()
        {
            uint32_t dataBegin[RENDERER_MAX_WORKER_THREADS] = {};
            uint32_t commandCount[RENDERER_MAX_WORKER_THREADS] = {};

            // Assign new commands
            commandList.Assign(m_workerThreadCount, dataBegin, commandCount);

            // Write assigned commands info to all worker threads
            for (var i = 0; i < m_workerThreadCount; i++)
            {
                m_workerThreads[i]->m_commandDataStart = dataBegin[i];
                m_workerThreads[i]->m_commandCount = commandCount[i];
            }
        }

        void RHIDirectX11::Initialize(Settings::_enum settings, ResetFlags::_enum flags)
        {
            m_settings = settings;
            m_resetFlags = flags;
            m_running = true;

            if (m_resetFlags & ResetFlags::TripleBuffered)
            {
                Internal::Fatal("D3D11 does not support tripple buffering, yet!");
                return;
            }

            D3D_FEATURE_LEVEL level;
            D3D_FEATURE_LEVEL featureLevels[] =
            {
                D3D_FEATURE_LEVEL_11_1,
                D3D_FEATURE_LEVEL_11_0
            };

            var hr = D3D11CreateDevice(nullptr,
                D3D_DRIVER_TYPE_HARDWARE,
                nullptr,
#ifdef _DEBUG
                D3D11_CREATE_DEVICE_DEBUG,
#else
                0,
#endif
                featureLevels,
                1,
                D3D11_SDK_VERSION,
                &m_device,
                &level,
                &m_deviceContext
            );

            if (FAILED(hr))
            {
                Internal::Fatal("Could not create device!");
                return;
            }

            // Get CPU count
            SYSTEM_INFO sysinfo;
            GetSystemInfo(&sysinfo);
            int cpuCount = sysinfo.dwNumberOfProcessors;

            if (m_settings & Settings::SingleThreaded)
                cpuCount = 1;

            // TODO: make single threaded model use main thread...

            // Spawn Worker Threads
            for (var i = 0; i < cpuCount && i < RENDERER_MAX_WORKER_THREADS; i++)
            {
                var workerInstance = new WorkerThreadInstance;

                // Create frame done signal event
                m_workerFrameEvents[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
                if (m_workerFrameEvents[i] == nullptr)
                {
                    Internal::Fatal("Failed to create event!");
                }

                // Create worker finish signal event
                m_workerFinishEvents[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
                if (m_workerFinishEvents[i] == nullptr)
                {
                    Internal::Fatal("Failed to create event!");
                }

                workerInstance->threadId = i;
                workerInstance->commandList = &commandList;

                // Initialize worker
                workerInstance->InitializeWorker();

                // Setup thread when we only need it
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
                if (thread) {

                    if (thread->thread.joinable())
                        thread->thread.join();

                    thread->Cleanup();
                    delete thread;
                }

                m_workerThreads[idx] = nullptr;
                idx++;
            }

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

            // Destroy command list
            commandList.Destroy();

            // Destroy D3D11 objects created on main-thread
            SafeRelease(m_deviceContext);
            SafeRelease(m_device);
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

            if (m_settings & Settings::SingleThreaded)
            {
                var thread = m_workerThreads[0];

                // Mannualy process frame now
                thread->ProcessFrame();

                CComPtr<ID3D11CommandList> commandList = nullptr;
                var hr = thread->m_context->FinishCommandList(FALSE, &commandList);

                if(FAILED(hr))
                {
                    Internal::Fatal("Failed to finish command list!");
                    return;
                }

                // Add current thread's command list
                commandLists.push_back(commandList);
            }
            else
            {
                // Wait for all worker threads
                WaitForMultipleObjects(m_workerThreadCount, m_workerFinishEvents, true, INFINITE);

                // Build command list array
                for (var thread : m_workerThreads)
                {
                    if (!thread)
                        break;

                    // Mannualy process frame now
                    thread->ProcessFrame();

                    CComPtr<ID3D11CommandList> commandList = nullptr;
                    var hr = thread->m_context->FinishCommandList(FALSE, &commandList);

                    if (FAILED(hr))
                    {
                        Internal::Fatal("Failed to finish command list!");
                        return;
                    }

                    // Add current thread's command list
                    commandLists.push_back(commandList);
                }
            }

            // Execute command lists
            for(var && commandList : commandLists)
            {
                m_deviceContext->ExecuteCommandList(commandList, TRUE);
            }

            // Present frame
            if (m_swapChain)
            {
                var hr = m_swapChain->Present(m_resetFlags & ResetFlags::VSync ? 1 : 0, 0);

                if (FAILED(hr))
                {
                    Internal::Fatal("Failed to present frame!");
                }
            }

            // Assign new commands for all threads
            assignCommands();

            if (!(m_settings & Settings::SingleThreaded))
            {
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
#pragma endregion 

#pragma region RHI interface implementation
        void RHIDirectX11::CreateWindowHandle(WindowHandle window, RenderBufferHandle renderBufferHandle, void* windowHandle)
        {
            waitForGPU();

            var hWnd = static_cast<HWND>(windowHandle);

            RECT windowRect = {};
            GetClientRect(hWnd, &windowRect);

            UINT width = windowRect.right - windowRect.left;
            UINT height = windowRect.bottom - windowRect.top;

            var bufferCount = m_resetFlags & ResetFlags::TripleBuffered ? FrameBufferCount : 1;

            var sampleCount = 1;

            if (m_resetFlags & ResetFlags::MSAAx2)
                sampleCount *= 2;

            if (m_resetFlags & ResetFlags::MSAAx4)
                sampleCount *= 4;

            IDXGIDevice* device;
            var hr = m_device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&device));

            if (FAILED(hr))
            {
                Internal::Fatal("Failed to query IDXGI device. (QueryInterface)");
                return;
            }

            IDXGIAdapter* adapter;
            hr = device->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&adapter));

            if (FAILED(hr))
            {
                Internal::Fatal("Failed to get parent(adapter) of device. (GetParent)");
                return;
            }

            IDXGIFactory* factory;
            hr = adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));

            if (FAILED(hr))
            {
                Internal::Fatal("Failed to get parent(factory) of adapter. (GetParent)");
                return;
            }

            // Create the Swap Chain
            DXGI_SAMPLE_DESC sampleDesc;
            sampleDesc.Count = sampleCount;
            sampleDesc.Quality = 0;

            DXGI_MODE_DESC backBufferDesc = {}; // this is to describe our display mode
            backBufferDesc.Width = width; // buffer width
            backBufferDesc.Height = height; // buffer height
            backBufferDesc.Format = m_settings & Settings::BGRAFrameBuffer ? DXGI_FORMAT_B8G8R8A8_UNORM : DXGI_FORMAT_R8G8B8A8_UNORM;

            // Describe and create the swap chain.
            DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
            swapChainDesc.BufferCount = bufferCount; // number of buffers we have
            swapChainDesc.BufferDesc = backBufferDesc; // our back buffer description
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // this says the pipeline will render to this swap chain
            swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // dxgi will discard the buffer (data) after we call present
            swapChainDesc.OutputWindow = hWnd; // handle to our window
            swapChainDesc.SampleDesc = sampleDesc; // our multi-sampling description
            swapChainDesc.Windowed = true; // set to true, then if in fullscreen must call SetFullScreenState with true for full screen to get uncapped fps

            IDXGISwapChain* tempSwapChain;
            hr = factory->CreateSwapChain(m_device, &swapChainDesc, &tempSwapChain);

            if (FAILED(hr))
            {
                Internal::Fatal("Could not create swap chain!");
                return;
            }

            m_swapChains[window.idx] = static_cast<IDXGISwapChain*>(tempSwapChain);

            // Apply the output as current
            m_swapChain = m_swapChains[window.idx];

            ID3D11RenderTargetView* renderTargets[FrameBufferCount] = {};
            for (var i = 0; i < bufferCount; i++)
            {
                ID3D11Resource* resource = nullptr;
                hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(&resource)); // NOTE: potential leak?

                if (FAILED(hr))
                {
                    Internal::Fatal("Could not create render target!");
                }

                m_device->CreateRenderTargetView(resource, nullptr, &renderTargets[i]);
            }

            // WARNING: No depth stencil view for back buffers!

            // TODO: DepthStencilView

            // Build 'proxy' render buffer
            m_renderBuffers[renderBufferHandle.idx] = RHIDirectX11_RenderBuffer::Create(m_device, bufferCount, renderTargets, nullptr);

            SafeRelease(factory);
            SafeRelease(adapter);
            SafeRelease(device);
        }

        void RHIDirectX11::CreateTexture2D(Texture2DHandle textureHandle, uint16_t width, uint16_t height, TextureFormat::_enum textureFormat)
        {
            //m_textures2d[textureHandle.idx] = RHIDirectX11_Texture2D::Create(m_device, width, height, textureFormat);
        }

        void RHIDirectX11::ResizeWindow(WindowHandle window, int width, int height)
        {
        }
#pragma endregion
    }
}

#endif