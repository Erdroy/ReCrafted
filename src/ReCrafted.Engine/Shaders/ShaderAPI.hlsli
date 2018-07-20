// ReCrafted (c) 2016-2018 Always Too Late

#ifndef SHADERAPI_HLSLI
#define SHADERAPI_HLSLI

// ==== PRESETS ====
// Preset: SURFACE (GBuffer filler)
#ifdef PRESET_SURFACE
#define USE_LOGZBUFFER
//#define USE_INVZBUFFER
//#define USE_UV
#define USE_VERTEXCOLOR
#endif // PRESET_SURFACE

// ==== API INCLUDES ====
#include "../API/Common.hlsli"
#include "../API/GBuffer.hlsli"
#include "../API/FullscreenQuad.hlsli"

// ==== COMMON INCLUDES ====
#include "../Common/Math.hlsli"
#include "../Common/Lighting.hlsli"

// ==== PRESET INCLUDES ====
#ifdef PRESET_SURFACE
#include "../Common/Surface.hlsli"
#endif

#endif // SHADERAPI_HLSLI