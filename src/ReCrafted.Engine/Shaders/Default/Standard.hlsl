// ReCrafted (c) 2016-2018 Always Too Late

#name "Standard"
#desc "Standard shader used for shading on almost every visible object in game"

#define SURFACE
#include "../RCShaderAPI.hlsli"

pass Default
{
    SetProfile(5.0);
    SetVertexShader(GBufferVSMain);
    SetPixelShader(GBufferPSMain);
}
