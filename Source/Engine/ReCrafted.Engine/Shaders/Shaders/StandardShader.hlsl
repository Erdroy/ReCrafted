// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#name "StandardShader"
#desc "StandardShader used for shading almost every visible object in game (excluding Terrain, as it has it's own shader)"

#define PRESET_SURFACE
#include "../ShaderAPI.hlsli"

pass Default
{
    BindDefaultConstantBuffer(SurfaceVSMain, SurfacePSMain);
    BindConstantBuffer(TestCB, SurfacePSMain);
   
    SetProfile(5.0);

    SetVertexShader(SurfaceVSMain);
    SetPixelShader(SurfacePSMain);
}
