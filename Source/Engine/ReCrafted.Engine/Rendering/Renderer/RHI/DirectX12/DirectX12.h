// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#if RENDERER_RHI_D3D12

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>

#define SafeRelease(x) if (x != NULL) { x->Release(); x = nullptr; }
#define SafeCloseHandle(x) if(x != NULL) { CloseHandle(x); x = NULL; }

#endif
