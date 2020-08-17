// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "RHIDirectX12.h"

#if RENDERER_RHI_D3D12

#include "../../../Renderer.hpp"
#include "../../../RendererConfig.h"

#include "Internal/RHIDirectX12_Texture2D.h"
#include "Internal/RHIDirectX12_RenderBuffer.h"

#include "DirectX12.h"
#include <thread>
#include <vector>

namespace Renderer
{
	namespace RHI
	{
		class WorkerThreadInstance;

		const int FrameBufferCount = 3;


        // == common ==
		volatile bool               m_running;
		int                         m_workerThreadCount;
		Settings::_enum             m_settings;
        RenderFlags::_enum           m_renderFlags;


        // == events ==
		HANDLE                      m_workerFinishEvents[RENDERER_MAX_RENDER_THREADS];
		HANDLE                      m_workerFrameEvents	[RENDERER_MAX_RENDER_THREADS];


        // == worker-related stuff ==
		WorkerThreadInstance*	    m_workerThreads     [RENDERER_MAX_RENDER_THREADS]               = {};


        // == resources ==
        IDXGISwapChain3*		    m_swapChains        [RENDERER_MAX_WINDOWS] = {};
        RHIDirectX12_RenderBuffer*  m_renderBuffers     [RENDERER_MAX_RENDER_BUFFERS]               = {};
        RHIDirectX12_Texture2D*     m_textures2d        [RENDERER_MAX_TEXTURES2D]                     = {};


        // == d3d12 resources ==
        ID3D12Device*               m_device = nullptr;
        ID3D12CommandQueue*         m_commandQueue = nullptr;
        IDXGIFactory4*              m_dxgiFactory = nullptr;
        ID3D12Debug*                m_debugController = nullptr;


        // == pointer-only resources (DO NOT RELEASE!) ==
		IDXGISwapChain3*            m_swapChain = nullptr;


		class WorkerThreadInstance
		{
			friend class RHIDirectX12;

		private:
			ID3D12CommandAllocator*     m_commandAllocator  [FrameBufferCount]	= {};
			ID3D12Fence*                m_fence             [FrameBufferCount]	= {};
			UINT64                      m_fenceValue        [FrameBufferCount]	= {};
			HANDLE                      m_fenceEvent                            = {};
			ID3D12GraphicsCommandList*  m_commandList                           = {};

			uint32_t m_commandDataStart = 0u;
			uint32_t m_commandCount = 0u;

		public:
			std::thread thread;

			int threadId;
			int frameIndex;
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
		};

		void WorkerThreadInstance::WaitForPreviousFrame()
		{
			HRESULT hr;

			// swap the current rtv buffer index so we draw on the correct buffer
			frameIndex = m_swapChain->GetCurrentBackBufferIndex();

			// if the current fence value is still less than "fenceValue", then we know the GPU has not finished executing
			// the command queue since it has not reached the "commandQueue->Signal(fence, fenceValue)" command
			if (m_fence[frameIndex]->GetCompletedValue() < m_fenceValue[frameIndex])
			{
				// we have the fence create an event which is signaled once the fence's current value is "fenceValue"
				hr = m_fence[frameIndex]->SetEventOnCompletion(m_fenceValue[frameIndex], m_fenceEvent);
				if (FAILED(hr))
				{
					Internal::Fatal("SetEventOnCompletion failed");
				}

				// We will wait until the fence has triggered the event that it's current value has reached "fenceValue". once it's value
				// has reached "fenceValue", we know the command queue has finished executing
				WaitForSingleObject(m_fenceEvent, INFINITE);
			}

			// increment fenceValue for next frame
			m_fenceValue[frameIndex]++;
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

			// create command allocators
			for (int i = 0; i < FrameBufferCount; i++)
			{
				hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator[i]));
				if (FAILED(hr))
				{
					Internal::Fatal("Failed to create command allocator!");
					return;
				}
			}

			// create the command list with the first allocator
			hr = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator[0], nullptr, IID_PPV_ARGS(&m_commandList));
			if (FAILED(hr))
			{
				Internal::Fatal("Failed to create command list!");
				return;
			}
			
			// close the command list now
			m_commandList->Close();

			// create the fences
			for (int i = 0; i < FrameBufferCount; i++)
			{
				hr = m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence[i]));
				if (FAILED(hr))
				{
					Internal::Fatal("Failed to create fence!");
					return;
				}
				m_fenceValue[i] = 0; // set the initial fence value to 0
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
			HRESULT hr;

			WaitForPreviousFrame();

			auto commandAllocator = m_commandAllocator[frameIndex];

			// reset the command allocator
			hr = commandAllocator->Reset();
			if (FAILED(hr))
			{
				Internal::Fatal("Failed to reset command allocator!");
				m_running = false;
			}

			// reset the command list (put it in recording state)
			hr = m_commandList->Reset(commandAllocator, nullptr);
			if (FAILED(hr))
			{
				Internal::Fatal("Failed to reset command list!");
				m_running = false;
			}

			ExecuteCommandList();
			
			// Close command list now,
			// it is ready to be executed
			m_commandList->Close();
		}

		void WorkerThreadInstance::ExecuteCommandList()
		{
			if (m_commandCount == 0u)
				return;

			auto position = m_commandDataStart;

			// Execute command list
			for(auto i = 0u; i < m_commandCount; i ++)
			{
				auto header = commandList->Read_CommandHeader(&position);
				position -= sizeof header;

				ExecuteCommand(header, &position);
			}
		}

		void WorkerThreadInstance::ExecuteCommand(CommandHeader::_enum header, uint32_t* position)
		{
#define DEFINE_COMMAND_EXECUTOR(name) \
            case CommandHeader::name: { \
                auto command = commandList->ReadCommand<Command_##name>(position);\
                Execute_##name(&command);\
                break; \
		    }

			switch (header)
			{
			case CommandHeader::Empty:
				break;
			case CommandHeader::ApplyWindow:
			{
				auto command = commandList->ReadCommand<Command_ApplyWindow>(position);
				m_swapChain = m_swapChains[command.window.idx];
				break;
			}
			case CommandHeader::DestroyWindow:
			{
				auto command = commandList->ReadCommand<Command_DestroyWindow>(position);
				
				auto idx = command.window.idx;

				SafeRelease(m_swapChains[idx]);

                auto renderBufferIdx = command.window.renderBuffer.idx;
                SafeRelease(m_renderBuffers[renderBufferIdx]);

				break;
			}
            DEFINE_COMMAND_EXECUTOR(ClearRenderBuffer);
            DEFINE_COMMAND_EXECUTOR(ApplyRenderBuffer);
            default: break;
			}
		}

		void WorkerThreadInstance::Cleanup()
		{
			for (auto i = 0; i < FrameBufferCount; i++)
			{
				SafeRelease(m_commandAllocator[i]);
				SafeRelease(m_fence[i]);
			}

			SafeRelease(m_commandList);
		}




		// ========== WorkerThreadInstance::Executors ==========

		void WorkerThreadInstance::Execute_ClearRenderBuffer(Command_ClearRenderBuffer* command) const
		{
			auto renderBufferIdx = command->renderBuffer.idx;

            // TODO: check if we can pass frame index (is it back buffer - render buffer)

            m_renderBuffers[renderBufferIdx]->Clear(m_commandList, command->color, 0x0, frameIndex);
		}

	    void WorkerThreadInstance::Execute_ApplyRenderBuffer(Command_ApplyRenderBuffer* command) const
	    {
            auto renderBufferIdx = command->renderBuffer.idx;

            // TODO: check if we can pass frame index (is it back buffer - render buffer)

            m_renderBuffers[renderBufferIdx]->Bind(m_commandList, frameIndex);
	    }




	    // ========== RHIDirectX12 ==========

		void RHIDirectX12::kickFrameEvent()
		{
			for (auto thread : m_workerThreads)
			{
				if (!thread)
					break;

				SetEvent(m_workerFrameEvents[thread->threadId]);
			}
		}

		void RHIDirectX12::waitForGPU()
		{
			// TODO: impl
		}

		void RHIDirectX12::assignCommands()
		{
			uint32_t dataBegin[RENDERER_MAX_RENDER_THREADS] = {};
			uint32_t commandCount[RENDERER_MAX_RENDER_THREADS] = {};

			// Assign new commands
			commandList.Assign(m_workerThreadCount, dataBegin, commandCount);

			// Write assigned commands info to all worker threads
			for(auto i = 0; i < m_workerThreadCount; i ++)
			{
				m_workerThreads[i]->m_commandDataStart = dataBegin[i];
				m_workerThreads[i]->m_commandCount = commandCount[i];
			}
		}

		void RHIDirectX12::Initialize(Settings::_enum settings, RenderFlags::_enum flags)
		{
			m_settings = settings;
            m_renderFlags = flags;
			m_running = true;

			HRESULT hr;

			hr = CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory));
			if (FAILED(hr))
			{
				Internal::Fatal("Failed to create DGXI Factory!");
				return;
			}

			auto adapterIndex = 0;
			auto adapterFound = false;

			// Find suitable adapter
			IDXGIAdapter1* adapter;
			while (m_dxgiFactory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND)
			{
				DXGI_ADAPTER_DESC1 desc;
				adapter->GetDesc1(&desc);

				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				{
					// we dont want a software device
					adapterIndex++; // add this line here. Its not currently in the downloadable project
					continue;
				}

				// we want a device that is compatible with direct3d 12 (feature level 11 or higher)
				hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
				if (SUCCEEDED(hr))
				{
					adapterFound = true;
					break;
				}

				adapterIndex++;
			}

			if (!adapterFound)
			{
				Internal::Fatal("Suitable dveice not found!");
				return;
			}

#ifdef _DEBUG
			// Enable the debug layer.
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_debugController))))
			{
				m_debugController->EnableDebugLayer();
			}
			else
			{
				Internal::Log("Failed to enable debug layer.");
			}
#endif

			// Create the device
			hr = D3D12CreateDevice(
				adapter,
				D3D_FEATURE_LEVEL_11_0,
				IID_PPV_ARGS(&m_device)
			);
			if (FAILED(hr))
			{
				Internal::Fatal("Could not create device!");
				return;
			}

			// Create the command queue
			D3D12_COMMAND_QUEUE_DESC cqDesc = {};

			hr = m_device->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&m_commandQueue));
			if (FAILED(hr))
			{
				Internal::Fatal("Could not create command queue!");
				return;
			}

			// Get CPU count
			SYSTEM_INFO sysinfo;
			GetSystemInfo(&sysinfo);
			int cpuCount = sysinfo.dwNumberOfProcessors;

			if (m_settings & Settings::SingleThreaded)
				cpuCount = 1;

			// Spawn Worker Threads
			for(auto i = 0; i < cpuCount && i < RENDERER_MAX_RENDER_THREADS; i++)
			{
				auto workerInstance = new WorkerThreadInstance;

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

		void RHIDirectX12::Shutdown()
		{
			m_running = false;

			// Kick threads
			kickFrameEvent();

			// Wait for worker threads to finish
			auto idx = 0;
			for (auto thread : m_workerThreads)
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
			for(auto i = 0; i < RENDERER_MAX_WINDOWS; i ++)
			{
				SafeRelease(m_swapChains[i]);
			}
            
            // Destroy render buffers (if not released)
            for (auto i = 0; i < RENDERER_MAX_RENDER_BUFFERS; i++)
            {
                SafeRelease(m_renderBuffers[i]);
            }

			// Destroy command list
			commandList.Destroy();

			// Destroy D3D12 objects created on main-thread
			SafeRelease(m_dxgiFactory);
			SafeRelease(m_commandQueue);
			SafeRelease(m_device);
			SafeRelease(m_debugController);
		}

		void RHIDirectX12::Frame()
		{
			static auto FrameCount = 0;
			static auto FirstFrame = true;
			static std::vector<ID3D12CommandList*> commandLists = {};

			if(FirstFrame)
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
				auto thread = m_workerThreads[0];

				// Mannualy process frame now
				thread->ProcessFrame();

				// Add current thread's command list
				commandLists.push_back(thread->m_commandList);
			}
			else 
			{
				// Wait for all worker threads
				WaitForMultipleObjects(m_workerThreadCount, m_workerFinishEvents, true, INFINITE);

				// Build command list array
				for (auto thread : m_workerThreads)
				{
					if (!thread)
						break;

					// Add current thread's command list
					commandLists.push_back(thread->m_commandList);
				}
			}

			// Execute command lists
			m_commandQueue->ExecuteCommandLists(static_cast<UINT>(commandLists.size()), commandLists.data());
			
			// Queue finish command now, it will tell us when GPU is done with this frame
			for (auto thread : m_workerThreads)
			{
				if (!thread)
					break;

				auto frameIdx = thread->frameIndex;

				// Set signal
				m_commandQueue->Signal(thread->m_fence[frameIdx], thread->m_fenceValue[frameIdx]);
			}

			// Present frame
			if (m_swapChain)
			{
				auto hr = m_swapChain->Present(1, 0);

				if(FAILED(hr))
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




        // ========== RESOURCES ==========

        void RHIDirectX12::CreateWindowHandle(WindowHandle window, RenderBufferHandle renderBufferHandle, void* windowHandle)
        {
            waitForGPU();

            auto hWnd = static_cast<HWND>(windowHandle);

            RECT windowRect = {};
            GetClientRect(hWnd, &windowRect);

            UINT width = windowRect.right - windowRect.left;
            UINT height = windowRect.bottom - windowRect.top;

            // Create the Swap Chain (tripple buffering)
            DXGI_MODE_DESC backBufferDesc = {}; // this is to describe our display mode
            backBufferDesc.Width = width; // buffer width
            backBufferDesc.Height = height; // buffer height
            backBufferDesc.Format = m_settings & Settings::BGRAFrameBuffer ? DXGI_FORMAT_B8G8R8A8_UNORM : DXGI_FORMAT_R8G8B8A8_UNORM;

            auto sampleCount = 1;

            if(m_renderFlags & RenderFlags::MSAAx2)
                sampleCount *= 2;

            if (m_renderFlags & RenderFlags::MSAAx4)
                sampleCount *= 4;

            DXGI_SAMPLE_DESC sampleDesc;
            sampleDesc.Count = sampleCount;
            sampleDesc.Quality = 0;
            auto bufferCount = m_renderFlags & RenderFlags::TripleBuffered ? FrameBufferCount : 1;

		    // Describe and create the swap chain.
            DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
            swapChainDesc.BufferCount = bufferCount; // number of buffers we have
            swapChainDesc.BufferDesc = backBufferDesc; // our back buffer description
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // this says the pipeline will render to this swap chain
		    swapChainDesc.SwapEffect = m_renderFlags & RenderFlags::TripleBuffered ? DXGI_SWAP_EFFECT_FLIP_DISCARD; // dxgi will discard the buffer (data) after we call present
            swapChainDesc.OutputWindow = hWnd; // handle to our window
            swapChainDesc.SampleDesc = sampleDesc; // our multi-sampling description
            swapChainDesc.Windowed = true; // set to true, then if in fullscreen must call SetFullScreenState with true for full screen to get uncapped fps

            IDXGISwapChain* tempSwapChain;

            auto hr = m_dxgiFactory->CreateSwapChain(
                m_commandQueue, // the queue will be flushed once the swap chain is created
                &swapChainDesc, // give it the swap chain description we created above
                &tempSwapChain // store the created swap chain in a temp IDXGISwapChain interface
            );

            if (FAILED(hr))
            {
                Internal::Fatal("Could not create swap chain!");
                return;
            }

            m_swapChains[window.idx] = static_cast<IDXGISwapChain3*>(tempSwapChain);

            // Apply the output as current
            m_swapChain = m_swapChains[window.idx];

            // Create heap
            D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
            rtvHeapDesc.NumDescriptors = bufferCount; // number of descriptors for this heap.
            rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // this heap is a render target view heap
            rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

            ID3D12DescriptorHeap* backBufferHeap = {};

            hr = m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&backBufferHeap));
            if (FAILED(hr))
            {
                Internal::Fatal("Could not create render target descriptor heap!");
            }

            // Update back buffer size
            auto incrementSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

            auto renderTargetHandle = backBufferHeap->GetCPUDescriptorHandleForHeapStart();

            ID3D12Resource* renderTargets[FrameBufferCount] = {};

            for (auto i = 0; i < bufferCount; i++)
            {
                hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i]));

                if (FAILED(hr))
                {
                    Internal::Fatal("Could not create render target!");
                }

                m_device->CreateRenderTargetView(renderTargets[i], nullptr, renderTargetHandle);
                renderTargetHandle.ptr += incrementSize;
            }

            // WARNING: No depth stencil view for back buffers!

            // Build 'proxy' render buffer
            m_renderBuffers[renderBufferHandle.idx] = RHIDirectX12_RenderBuffer::Create(m_device, incrementSize, backBufferHeap, bufferCount, renderTargets, nullptr);
        }

	    void RHIDirectX12::CreateTexture2D(Texture2DHandle textureHandle, uint16_t width, uint16_t height, TextureFormat::_enum textureFormat)
	    {
            m_textures2d[textureHandle.idx] = RHIDirectX12_Texture2D::Create(m_device, width, height, textureFormat);
	    }

	    void RHIDirectX12::ResizeWindow(WindowHandle window, int width, int height)
	    {
            // TODO: resize
	    }
	}
}
#endif
