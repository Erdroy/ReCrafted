// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef DIRECTX12_H
#define DIRECTX12_H

#if RENDERER_RHI_D3D12

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>

#define SafeRelease(x) if (x != NULL) { x->Release(); x = nullptr; }
#define SafeCloseHandle(x) if(x != NULL) { CloseHandle(x); x = NULL; }

#endif

#endif // DIRECTX12_H
