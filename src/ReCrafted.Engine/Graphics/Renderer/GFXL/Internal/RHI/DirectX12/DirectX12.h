// GFXL - Graphics Library (c) 2016-2017 Damian 'Erdroy' Korczowski

#pragma once

#ifndef DIRECTX12_H
#define DIRECTX12_H

#if GFXL_RENDERER_D3D12

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>

#define SafeRelease(x) if (x != NULL) { x->Release(); x = nullptr; }
#define SafeCloseHandle(x) if(x != NULL) { CloseHandle(x); x = NULL; }

#endif

#endif // DIRECTX12_H
