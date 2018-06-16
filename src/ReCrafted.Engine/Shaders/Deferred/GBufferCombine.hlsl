// ReCrafted (c) 2016-2018 Always Too Late

#name "GBuffer Combine"
#desc "GBuffer combine shader"

#define USE_FULLSCREENQUAD
#define USE_GBUFFERSAMPLING
#include "../ShaderAPI.hlsli"

[Target(CombinePSMain)]
cbuffer Data : register(b0)
{
    float3 lightDir;
    float3 cameraPos;
    float4 viewInfo; // viewInfo x (nearPlane), y (farPlane), z (1.0 / projection[0, 0]), w (1.0 / projection[1, 1])
};

float3 CalculateLighting(float3 normal) 
{
    float3 dir = normalize(lightDir);
    float nDotL = dot(normal, dir);
    float3 lightColor = float3(1.0f, 0.95f, 0.9f);

    return lightColor * nDotL + float3(0.35f, 0.35f, 0.35f);
}

float linearize_depth(in float depth) {
    float a = viewInfo.y / (viewInfo.y - viewInfo.x);
    float b = viewInfo.y * viewInfo.x / (viewInfo.x - viewInfo.y);
    return a + b / depth;
}

/// <summary>
/// Pixel Shader Function
/// CombinePSMain
/// </summary>
float4 CombinePSMain(in QuadPS i) : SV_Target0
{
    GBuffer gbuffer = SampleGBuffer(i.UV);
    float3 color = gbuffer.Color;

    color *= CalculateLighting(gbuffer.Normal);

    //float3 viewDir = - cameraPos;
    //float fresnel = pow((1.0f - saturate(dot(normalize(gbuffer.Normal), normalize(viewDir)))), 1.0f);
    //color *= fresnel; 
    
    // reconstruct depth ((exp(depth / FC) - 1.0) / C);
    //const float C = 1.0f;
    //const float FC = 1.0 / log(viewInfo.y * C + 1);
    //color = linearize_depth((exp(gbuffer.Depth / FC) - 1.0) / C);

    return float4(color, 1.0f);
}

pass Default
{
    SetProfile(5.0);
    SetVertexShader(QuadVSMain);
    SetPixelShader(CombinePSMain);
}
