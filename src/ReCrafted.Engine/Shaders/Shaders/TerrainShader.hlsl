// ReCrafted (c) 2016-2018 Always Too Late

#name "TerrainShader"
#desc "Terrain Shader - Used for drawing planetary terrain. Implements own GeometryShader which adds more LowPoly effect."

#define PRESET_SURFACE
#include "../ShaderAPI.hlsli"

[maxvertexcount(3)]
void TerrainGSMain(triangle SurfaceVSOutput input[3], inout TriangleStream<SurfacePSInput> OutputStream)
{
    SurfacePSInput output;
    
    // Calculate output normal for all vertices
    output.Normal = normalize(input[0].Normal + input[1].Normal + input[2].Normal);

    // Add 1st triangle
    output.Position = input[0].Position;
#ifdef USE_LOGZBUFFER
    output.LogZ = input[0].LogZ;
#endif
#ifdef USE_VERTEXCOLOR
    output.Color = input[0].Color;
#endif // USE_VERTEXCOLOR
    OutputStream.Append(output);

    // Add 2nd triangle
    output.Position = input[1].Position;
#ifdef USE_LOGZBUFFER
    output.LogZ = input[1].LogZ;
#endif
#ifdef USE_VERTEXCOLOR
    output.Color = input[1].Color;
#endif // USE_VERTEXCOLOR
    OutputStream.Append(output);

    // Add 3rd triangle
    output.Position = input[2].Position;
#ifdef USE_LOGZBUFFER
    output.LogZ = input[2].LogZ;
#endif
#ifdef USE_VERTEXCOLOR
    output.Color = input[2].Color;
#endif // USE_VERTEXCOLOR
    OutputStream.Append(output);

    OutputStream.RestartStrip();
}

pass Default 
{
    BindDefaultConstantBuffer(SurfaceVSMain, SurfacePSMain);

    SetProfile(5.0);
    SetVertexShader(SurfaceVSMain);
    SetGeometryShader(TerrainGSMain);
    SetPixelShader(SurfacePSMain);
}