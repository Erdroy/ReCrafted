// ReCrafted (c) 2016-2018 Always Too Late

#name "GBuffer Standard"
#desc "GBuffer Standard shader used for shading on almost every visible object in game"

#define PRESET_SURFACE
#include "../RCShaderAPI.hlsli"

pass Default
{
    SetProfile(5.0);
    SetVertexShader(SurfaceVSMain);
    SetPixelShader(SurfacePSMain);
}
