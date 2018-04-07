// GFXL - Graphics Library (c) 2016-2017 Damian 'Erdroy' Korczowski

#pragma once

#ifndef DIRECTX11_H
#define DIRECTX11_H

#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <atlbase.h>

#define SafeRelease(x) if (x != NULL) { x->Release(); x = nullptr; }
#define SafeCloseHandle(x) if(x != NULL) { CloseHandle(x); x = NULL; }

#endif // DIRECTX11_H
