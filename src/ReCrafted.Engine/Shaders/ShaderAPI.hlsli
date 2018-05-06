// ReCrafted (c) 2016-2018 Always Too Late

#ifndef RCSHADERAPI_HLSLI
#define RCSHADERAPI_HLSLI

// ==== PRESETS ====
// Preset: SURFACE (GBuffer filler)
#ifdef PRESET_SURFACE
#define USE_3DSPACE
#define USE_LOGZBUFFER
//#define USE_UV
#define USE_VERTEXCOLOR
#endif // PRESET_SURFACE

// ==== API INCLUDES ====
#include "../API/Common.hlsli"
#include "../API/GBuffer.hlsli"
#include "../API/FullscreenQuad.hlsli"

// ==== PRESET INCLUDES ====
#ifdef PRESET_SURFACE
#include "../API/Surface.hlsli"
#endif

#endif // RCSHADERAPI_HLSLI